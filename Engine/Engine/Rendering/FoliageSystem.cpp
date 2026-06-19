#include "EnginePch.h"
#include "FoliageSystem.h"

#include "GPUResourceManager.h"
#include "IndexBufferSystem.h"
#include "Mesh.h"
#include "MeshSystem.h"
#include "MeshUtils.h"
#include "VertexBufferSystem.h"
#include "Engine/Engine.h"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"

namespace
{
    // Deterministic per-instance pseudo-random in [0,1).
    float Hash01(uint inSeed)
    {
        inSeed = (inSeed ^ 61u) ^ (inSeed >> 16);
        inSeed *= 9u;
        inSeed = inSeed ^ (inSeed >> 4);
        inSeed *= 0x27d4eb2du;
        inSeed = inSeed ^ (inSeed >> 15);
        return static_cast<float>(inSeed & 0xFFFFFFu) / static_cast<float>(0x1000000u);
    }

    uint PackColorRGBA8(const glm::vec3& inColor)
    {
        auto to8 = [](float inValue) -> uint
        {
            const float clamped = inValue < 0.0f ? 0.0f : (inValue > 1.0f ? 1.0f : inValue);
            return static_cast<uint>(clamped * 255.0f + 0.5f);
        };
        return to8(inColor.r) | (to8(inColor.g) << 8) | (to8(inColor.b) << 16) | (255u << 24);
    }

    constexpr float fieldSize = 6000.0f; // world-space extent the foliage field covers
}

FoliageSystem::FoliageSystem() = default;

FoliageSystem::~FoliageSystem()
{
    // Buffers registered by alias are NOT owned by GPUResourceManager, so we free them here.
    VulkanAllocator::DestroyBuffer_TS(myInstanceBuffer);
    VulkanAllocator::DestroyBuffer_TS(myHeaderBuffer);
    VulkanAllocator::DestroyBuffer_TS(myIndirectBuffer);
    VulkanAllocator::DestroyBuffer_TS(myCountBuffer);
    VulkanAllocator::DestroyBuffer_TS(myPerDrawBuffer);
}

void FoliageSystem::Init()
{
    CreateBuffers();
    CreateFoliageTypes();
    GenerateInstances();
}

void FoliageSystem::CreateBuffers()
{
    GPUResourceManager* resourceManager = GPUResourceManager::Get();

    // Compact per-instance pool. Read by the cull shader.
    myInstanceBuffer = VulkanAllocator::AllocateBuffer_TS("Foliage Instance Buffer",
        VulkanBuffer::StorageBufferCreateInfo(sizeof(FoliageInstanceData) * myCapacity),
        VMA_MEMORY_USAGE_AUTO);
    resourceManager->RegisterBuffer(myInstanceBuffer, {"FoliageInstanceData", "inFoliageInstances"});

    // Live instance count, drives the cull dispatch bound size.
    myHeaderBuffer = VulkanAllocator::AllocateBuffer_TS("Foliage Header Buffer",
        VulkanBuffer::UniformBufferCreateInfo(sizeof(FoliageSceneHeader)),
        VMA_MEMORY_USAGE_AUTO);
    resourceManager->RegisterBuffer(myHeaderBuffer, {"FoliageSceneHeader", "inFoliageHeader"});

    // Indirect draw commands written by the cull pass, consumed by drawIndexedIndirectCount.
    myIndirectBuffer = VulkanAllocator::AllocateBuffer_TS("Foliage Indirect Buffer",
        vk::BufferCreateInfo()
            .setSize(sizeof(vk::DrawIndexedIndirectCommand) * myCapacity)
            .setUsage(vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst)
            .setSharingMode(vk::SharingMode::eExclusive),
        VMA_MEMORY_USAGE_AUTO);
    resourceManager->RegisterBuffer(myIndirectBuffer, {"FoliageDrawCommand", "outFoliageIndirectBuffer"});

    // Single uint draw counter. Cleared by the foliage pre-pass, incremented by the cull pass.
    myCountBuffer = VulkanAllocator::AllocateBuffer_TS("Foliage Count Buffer",
        vk::BufferCreateInfo()
            .setSize(sizeof(uint))
            .setUsage(vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst)
            .setSharingMode(vk::SharingMode::eExclusive),
        VMA_MEMORY_USAGE_AUTO);
    resourceManager->RegisterBuffer(myCountBuffer, {"outFoliageCountBuffer"});

    // Per-draw payload (world matrix + material indices + tint) produced by cull, read by the foliage VS/PS.
    myPerDrawBuffer = VulkanAllocator::AllocateBuffer_TS("Foliage PerDraw Buffer",
        VulkanBuffer::StorageBufferCreateInfo(sizeof(FoliagePerDrawData) * myCapacity),
        VMA_MEMORY_USAGE_AUTO);
    resourceManager->RegisterBuffer(myPerDrawBuffer, {"FoliagePerDrawData", "outFoliagePerDrawData"});
}

void FoliageSystem::CreateFoliageTypes()
{
    // Three visually distinct demo types: short dense grass, taller sparser ferns,
    // and small sparse flowers. They differ by mesh size, tint and density. Real
    // materials are assigned per type once the editor/asset work lands.
    {
        FoliageType& grass = myTypes.Emplace();
        grass.myTint = { 0.22f, 0.55f, 0.16f };
        grass.myMinScale = 60.0f;
        grass.myMaxScale = 120.0f;
        grass.myInstancesPerAxis = 48;
        CreateCrossQuadMesh(0.6f, 1.0f, grass);
    }
    {
        FoliageType& fern = myTypes.Emplace();
        fern.myTint = { 0.14f, 0.40f, 0.22f };
        fern.myMinScale = 150.0f;
        fern.myMaxScale = 260.0f;
        fern.myInstancesPerAxis = 26;
        CreateCrossQuadMesh(1.2f, 1.4f, fern);
    }
    {
        FoliageType& flower = myTypes.Emplace();
        flower.myTint = { 0.85f, 0.35f, 0.65f };
        flower.myMinScale = 50.0f;
        flower.myMaxScale = 90.0f;
        flower.myInstancesPerAxis = 18;
        CreateCrossQuadMesh(0.5f, 0.8f, flower);
    }
}

void FoliageSystem::CreateCrossQuadMesh(float inWidth, float inHeight, FoliageType& outType)
{
    List<Vertex> vertices;
    List<uint> indices;

    const float halfWidth = inWidth * 0.5f;

    auto addQuad = [&](const glm::vec3& inRight, const glm::vec3& inNormal)
    {
        const uint base = static_cast<uint>(vertices.size());
        const glm::vec3 up = { 0.0f, inHeight, 0.0f };

        // 0:bottom-left 1:bottom-right 2:top-right 3:top-left.
        const glm::vec3 corners[4] = {
            -halfWidth * inRight,
             halfWidth * inRight,
             halfWidth * inRight + up,
            -halfWidth * inRight + up,
        };
        const glm::vec2 uvs[4] = { {0, 1}, {1, 1}, {1, 0}, {0, 0} };

        for (int i = 0; i < 4; ++i)
        {
            Vertex vertex{};
            vertex.myPosition = corners[i];
            vertex.myColor = 0;
            vertex.myNormal = inNormal;
            vertex.myTangents = inRight;
            vertex.myBinormals = { 0.0f, 1.0f, 0.0f };
            vertex.myTexCoords[0] = uvs[i];
            vertex.myTexCoords[1] = uvs[i];
            vertices.Add(vertex);
        }

        // Front winding.
        indices.Add(base + 0); indices.Add(base + 1); indices.Add(base + 2);
        indices.Add(base + 0); indices.Add(base + 2); indices.Add(base + 3);
        // Back winding (double-sided).
        indices.Add(base + 0); indices.Add(base + 2); indices.Add(base + 1);
        indices.Add(base + 0); indices.Add(base + 3); indices.Add(base + 2);
    };

    addQuad({ 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
    addQuad({ 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f });

    const uint vertexSize = vertices.size() * sizeof(Vertex);
    VulkanBuffer* stagingVertexBuffer = VulkanAllocator::AllocateBuffer_TS("VulkanBuffer-Staging", VulkanBuffer::StagingCreateInfo(vertexSize), VMA_MEMORY_USAGE_AUTO, true);
    stagingVertexBuffer->SetData(vertices.data(), vertexSize);

    const uint indexSize = indices.size() * sizeof(uint);
    VulkanBuffer* stagingIndexBuffer = VulkanAllocator::AllocateBuffer_TS("VulkanBuffer-Staging", VulkanBuffer::StagingCreateInfo(indexSize), VMA_MEMORY_USAGE_AUTO, true);
    stagingIndexBuffer->SetData(indices.data(), indexSize);

    outType.myVertexBuffer = Engine::GetEngineSystem<VertexBufferSystem>().UploadVertexBuffer(stagingVertexBuffer, vertices.size());
    VulkanAllocator::DestroyBuffer_TS(stagingVertexBuffer);

    outType.myIndexBuffer = Engine::GetEngineSystem<IndexBufferSystem>().UploadIndexBuffer(stagingIndexBuffer, indices.size());
    VulkanAllocator::DestroyBuffer_TS(stagingIndexBuffer);

    const glm::vec4 sphereBounds = MeshUtils::CalculateSphereBounds(vertices);
    outType.myMesh = Engine::GetEngineSystem<MeshSystem>().UploadMesh(outType.myVertexBuffer, outType.myIndexBuffer, sphereBounds);
}

void FoliageSystem::GenerateInstances()
{
    List<FoliageInstanceData> instances;
    instances.Reserve(myCapacity);

    for (int typeIndex = 0; typeIndex < myTypes.size(); ++typeIndex)
    {
        const FoliageType& type = myTypes[typeIndex];
        if (!type.myMesh)
            continue;

        const int side = type.myInstancesPerAxis;
        const uint tintPacked = PackColorRGBA8(type.myTint);
        const uint meshHandle = type.myMesh->GetHandle();
        const uint typeSeed = static_cast<uint>(typeIndex) * 9176u;

        for (int z = 0; z < side; ++z)
        {
            for (int x = 0; x < side; ++x)
            {
                if (instances.size() >= static_cast<int>(myCapacity))
                    break;

                const uint seed = typeSeed + static_cast<uint>(z * side + x);
                const float cellU = side > 1 ? static_cast<float>(x) / static_cast<float>(side - 1) : 0.5f;
                const float cellV = side > 1 ? static_cast<float>(z) / static_cast<float>(side - 1) : 0.5f;
                const float jitter = fieldSize / static_cast<float>(side) * 0.4f;

                FoliageInstanceData instance{};
                instance.myPosition = {
                    (cellU - 0.5f) * fieldSize + (Hash01(seed * 3u + 0u) - 0.5f) * 2.0f * jitter,
                    0.0f,
                    (cellV - 0.5f) * fieldSize + (Hash01(seed * 3u + 1u) - 0.5f) * 2.0f * jitter,
                };
                instance.myScale = type.myMinScale + Hash01(seed * 3u + 2u) * (type.myMaxScale - type.myMinScale);
                instance.myRotationY = Hash01(seed) * 6.28318530718f;
                instance.myMeshIndex = meshHandle;
                instance.myTintPacked = tintPacked;
                instances.Add(instance);
            }
        }
    }

    myNumInstances = static_cast<uint>(instances.size());
    check(myNumInstances <= myCapacity);

    if (myNumInstances > 0)
    {
        const uint instanceDataSize = myNumInstances * sizeof(FoliageInstanceData);
        myInstanceBuffer->SetData(instances.data(), instanceDataSize);
    }

    FoliageSceneHeader header{};
    header.myNumInstances = myNumInstances;
    myHeaderBuffer->SetData(&header, sizeof(FoliageSceneHeader));
}
