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
    CreateFoliageMesh();
    GenerateTestInstances();
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

    // Per-draw payload (world matrix + material indices) produced by cull, read by the foliage VS/PS.
    myPerDrawBuffer = VulkanAllocator::AllocateBuffer_TS("Foliage PerDraw Buffer",
        VulkanBuffer::StorageBufferCreateInfo(sizeof(FoliagePerDrawData) * myCapacity),
        VMA_MEMORY_USAGE_AUTO);
    resourceManager->RegisterBuffer(myPerDrawBuffer, {"FoliagePerDrawData", "outFoliagePerDrawData"});
}

void FoliageSystem::CreateFoliageMesh()
{
    // A "grass" cross: two perpendicular vertical quads, made double-sided so the
    // foliage shows regardless of the pass's front-face cull mode.
    List<Vertex> vertices;
    List<uint> indices;

    auto addQuad = [&](const glm::vec3& inRight, const glm::vec3& inNormal)
    {
        const uint base = static_cast<uint>(vertices.size());
        const glm::vec3 up = { 0.0f, 1.0f, 0.0f };

        // 0:bottom-left 1:bottom-right 2:top-right 3:top-left, height 0..1.
        const glm::vec3 corners[4] = {
            -0.5f * inRight,
             0.5f * inRight,
             0.5f * inRight + up,
            -0.5f * inRight + up,
        };
        const glm::vec2 uvs[4] = { {0, 1}, {1, 1}, {1, 0}, {0, 0} };

        for (int i = 0; i < 4; ++i)
        {
            Vertex vertex{};
            vertex.myPosition = corners[i];
            vertex.myColor = 0;
            vertex.myNormal = inNormal;
            vertex.myTangents = inRight;
            vertex.myBinormals = up;
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

    myVertexBuffer = Engine::GetEngineSystem<VertexBufferSystem>().UploadVertexBuffer(stagingVertexBuffer, vertices.size());
    VulkanAllocator::DestroyBuffer_TS(stagingVertexBuffer);

    myIndexBuffer = Engine::GetEngineSystem<IndexBufferSystem>().UploadIndexBuffer(stagingIndexBuffer, indices.size());
    VulkanAllocator::DestroyBuffer_TS(stagingIndexBuffer);

    const glm::vec4 sphereBounds = MeshUtils::CalculateSphereBounds(vertices);
    myMesh = Engine::GetEngineSystem<MeshSystem>().UploadMesh(myVertexBuffer, myIndexBuffer, sphereBounds);
}

void FoliageSystem::GenerateTestInstances()
{
    // Deterministic per-instance pseudo-random in [0,1).
    auto hash01 = [](uint inSeed) -> float
    {
        inSeed = (inSeed ^ 61u) ^ (inSeed >> 16);
        inSeed *= 9u;
        inSeed = inSeed ^ (inSeed >> 4);
        inSeed *= 0x27d4eb2du;
        inSeed = inSeed ^ (inSeed >> 15);
        return static_cast<float>(inSeed & 0xFFFFFFu) / static_cast<float>(0x1000000u);
    };

    constexpr int gridSide = 60;          // 60x60 = 3600 instances (<= myCapacity)
    constexpr float spacing = 100.0f;     // world units between instances
    constexpr float jitter = 40.0f;       // random offset within a cell
    const float halfExtent = (gridSide - 1) * spacing * 0.5f;

    List<FoliageInstanceData> instances;
    instances.Reserve(gridSide * gridSide);

    for (int z = 0; z < gridSide; ++z)
    {
        for (int x = 0; x < gridSide; ++x)
        {
            const uint seed = static_cast<uint>(z * gridSide + x);

            FoliageInstanceData instance{};
            instance.myPosition = {
                x * spacing - halfExtent + (hash01(seed * 3u + 0u) - 0.5f) * 2.0f * jitter,
                0.0f,
                z * spacing - halfExtent + (hash01(seed * 3u + 1u) - 0.5f) * 2.0f * jitter,
            };
            instance.myScale = 80.0f + hash01(seed * 3u + 2u) * 80.0f;        // 80..160 tall
            instance.myRotationY = hash01(seed) * 6.28318530718f;             // 0..2pi yaw
            instance.myMeshIndex = myMesh->GetHandle();
            instances.Add(instance);
        }
    }

    myNumInstances = static_cast<uint>(instances.size());
    check(myNumInstances <= myCapacity);

    const uint instanceDataSize = myNumInstances * sizeof(FoliageInstanceData);
    myInstanceBuffer->SetData(instances.data(), instanceDataSize);

    FoliageSceneHeader header{};
    header.myNumInstances = myNumInstances;
    myHeaderBuffer->SetData(&header, sizeof(FoliageSceneHeader));
}
