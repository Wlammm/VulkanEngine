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

    float Clamp01(float inValue)
    {
        return inValue < 0.0f ? 0.0f : (inValue > 1.0f ? 1.0f : inValue);
    }

    uint PackColorRGBA8(const glm::vec3& inColor)
    {
        auto to8 = [](float inValue) -> uint
        {
            return static_cast<uint>(Clamp01(inValue) * 255.0f + 0.5f);
        };
        return to8(inColor.r) | (to8(inColor.g) << 8) | (to8(inColor.b) << 16) | (255u << 24);
    }
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
    VulkanAllocator::DestroyBuffer_TS(myScalabilityBuffer);
}

void FoliageSystem::Init()
{
    CreateBuffers();
    CreateFoliageTypes();
    RegenerateInstances();
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

    // Runtime scalability knobs, read by the cull pass.
    myScalabilityBuffer = VulkanAllocator::AllocateBuffer_TS("Foliage Scalability Buffer",
        VulkanBuffer::UniformBufferCreateInfo(sizeof(FoliageScalabilitySettings)),
        VMA_MEMORY_USAGE_AUTO);
    resourceManager->RegisterBuffer(myScalabilityBuffer, {"FoliageScalabilitySettings", "inFoliageScalability"});
    myScalabilityBuffer->SetData(&mySettings, sizeof(FoliageScalabilitySettings));
}

void FoliageSystem::SetTypeDensity(int inTypeIndex, float inValue)
{
    if (inTypeIndex < 0 || inTypeIndex >= myTypes.size())
        return;

    const float clamped = Clamp01(inValue);
    List<float>& density = myTypes[inTypeIndex].myDensityMap;
    for (int i = 0; i < density.size(); ++i)
        density[i] = clamped;
}

void FoliageSystem::SetScalability(float inMaxDistanceScale, float inDensityScale)
{
    mySettings.myGlobalMaxDistanceScale = inMaxDistanceScale;
    mySettings.myGlobalDensityScale = inDensityScale;
    myScalabilityBuffer->SetData(&mySettings, sizeof(FoliageScalabilitySettings));
}

void FoliageSystem::CreateFoliageTypes()
{
    // Three visually distinct demo types: short dense grass, taller sparser ferns,
    // and small sparse flowers. They differ by mesh size, tint and density. Real
    // materials are assigned per type once the editor/asset work lands.
    auto initType = [](FoliageType& inType, float inDefaultDensity)
    {
        inType.myDensityMap.Resize(FoliageSystem::myDensityRes * FoliageSystem::myDensityRes);
        for (int i = 0; i < inType.myDensityMap.size(); ++i)
            inType.myDensityMap[i] = inDefaultDensity;
    };

    {
        FoliageType& grass = myTypes.Emplace();
        grass.myName = "Grass";
        grass.myTint = { 0.22f, 0.55f, 0.16f };
        grass.myMinScale = 60.0f;
        grass.myMaxScale = 120.0f;
        grass.myDensityMultiplier = 1.0f;
        initType(grass, 0.45f);
        CreateCrossQuadMesh(0.6f, 1.0f, grass);
    }
    {
        FoliageType& fern = myTypes.Emplace();
        fern.myName = "Fern";
        fern.myTint = { 0.14f, 0.40f, 0.22f };
        fern.myMinScale = 150.0f;
        fern.myMaxScale = 260.0f;
        fern.myDensityMultiplier = 0.5f;
        initType(fern, 0.25f);
        CreateCrossQuadMesh(1.2f, 1.4f, fern);
    }
    {
        FoliageType& flower = myTypes.Emplace();
        flower.myName = "Flower";
        flower.myTint = { 0.85f, 0.35f, 0.65f };
        flower.myMinScale = 50.0f;
        flower.myMaxScale = 90.0f;
        flower.myDensityMultiplier = 0.3f;
        initType(flower, 0.2f);
        CreateCrossQuadMesh(0.5f, 0.8f, flower);
    }
}

glm::vec2 FoliageSystem::CellToWorld(int inCellX, int inCellZ) const
{
    const float u = (static_cast<float>(inCellX) + 0.5f) / static_cast<float>(myDensityRes);
    const float v = (static_cast<float>(inCellZ) + 0.5f) / static_cast<float>(myDensityRes);
    return { (u - 0.5f) * myFieldSize, (v - 0.5f) * myFieldSize };
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

void FoliageSystem::PaintDensity(int inTypeIndex, const glm::vec2& inWorldXZ, float inWorldRadius, float inStrength, bool inAdditive)
{
    if (inTypeIndex < 0 || inTypeIndex >= myTypes.size() || inWorldRadius <= 0.0f)
        return;

    FoliageType& type = myTypes[inTypeIndex];
    const float cellSize = myFieldSize / static_cast<float>(myDensityRes);
    const int radiusCells = static_cast<int>(inWorldRadius / cellSize) + 1;

    const float cu = inWorldXZ.x / myFieldSize + 0.5f;
    const float cv = inWorldXZ.y / myFieldSize + 0.5f;
    const int centerX = static_cast<int>(std::floor(cu * myDensityRes));
    const int centerZ = static_cast<int>(std::floor(cv * myDensityRes));

    const float sign = inAdditive ? 1.0f : -1.0f;

    for (int cz = centerZ - radiusCells; cz <= centerZ + radiusCells; ++cz)
    {
        if (cz < 0 || cz >= myDensityRes)
            continue;

        for (int cx = centerX - radiusCells; cx <= centerX + radiusCells; ++cx)
        {
            if (cx < 0 || cx >= myDensityRes)
                continue;

            const glm::vec2 cellWorld = CellToWorld(cx, cz);
            const float dist = glm::length(cellWorld - inWorldXZ);
            if (dist > inWorldRadius)
                continue;

            const float falloff = 1.0f - dist / inWorldRadius;
            const int idx = cz * myDensityRes + cx;
            type.myDensityMap[idx] = Clamp01(type.myDensityMap[idx] + sign * inStrength * falloff);
        }
    }
}

bool FoliageSystem::RaycastHeightfield(const glm::vec3& inOrigin, const glm::vec3& inDirection, glm::vec3& outHitPos) const
{
    const glm::vec3 dir = glm::normalize(inDirection);

    constexpr float maxDistance = 100000.0f;
    constexpr float step = 100.0f;

    glm::vec3 prevP = inOrigin;
    float prevDiff = inOrigin.y - myHeightfield.GetHeight({ inOrigin.x, inOrigin.z });

    for (float t = step; t <= maxDistance; t += step)
    {
        const glm::vec3 p = inOrigin + dir * t;
        const float diff = p.y - myHeightfield.GetHeight({ p.x, p.z });

        if (prevDiff > 0.0f && diff <= 0.0f)
        {
            // Refine the crossing between prevP (above) and p (below) with a binary search.
            glm::vec3 a = prevP;
            glm::vec3 b = p;
            for (int i = 0; i < 16; ++i)
            {
                const glm::vec3 mid = (a + b) * 0.5f;
                const float midDiff = mid.y - myHeightfield.GetHeight({ mid.x, mid.z });
                if (midDiff > 0.0f)
                    a = mid;
                else
                    b = mid;
            }
            outHitPos = (a + b) * 0.5f;
            return true;
        }

        prevP = p;
        prevDiff = diff;
    }

    return false;
}

void FoliageSystem::RegenerateInstances()
{
    List<FoliageInstanceData> instances;
    instances.Reserve(myCapacity);

    bool full = false;

    for (int typeIndex = 0; typeIndex < myTypes.size() && !full; ++typeIndex)
    {
        const FoliageType& type = myTypes[typeIndex];
        if (!type.myMesh)
            continue;

        const uint tintPacked = PackColorRGBA8(type.myTint);
        const uint meshHandle = type.myMesh->GetHandle();
        const uint typeSeed = static_cast<uint>(typeIndex) * 0x9E3779B1u;
        const float cellSize = myFieldSize / static_cast<float>(myDensityRes);

        for (int cz = 0; cz < myDensityRes && !full; ++cz)
        {
            for (int cx = 0; cx < myDensityRes; ++cx)
            {
                if (instances.size() >= static_cast<int>(myCapacity))
                {
                    full = true;
                    break;
                }

                const int cellIdx = cz * myDensityRes + cx;
                const float probability = type.myDensityMap[cellIdx] * type.myDensityMultiplier;
                const uint seed = typeSeed + static_cast<uint>(cellIdx);

                if (Hash01(seed) >= probability)
                    continue;

                const glm::vec2 cellWorld = CellToWorld(cx, cz);
                const float px = cellWorld.x + (Hash01(seed * 3u + 0u) - 0.5f) * cellSize;
                const float pz = cellWorld.y + (Hash01(seed * 3u + 1u) - 0.5f) * cellSize;
                const float py = myHeightfield.GetHeight({ px, pz });

                FoliageInstanceData instance{};
                instance.myPosition = { px, py, pz };
                instance.myScale = type.myMinScale + Hash01(seed * 3u + 2u) * (type.myMaxScale - type.myMinScale);
                instance.myRotationY = Hash01(seed * 7u) * 6.28318530718f;
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
