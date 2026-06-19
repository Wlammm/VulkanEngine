#pragma once
#include "Engine/Shaders/Shared/FoliageStructs.hpp"
#include "Engine/System/System.h"

class Mesh;
class VulkanBuffer;
class VertexBufferHandle;
class IndexBufferHandle;

/*
 * Phase 1 of the GPU-driven foliage system (see docs/foliage-rendering-plan.md).
 *
 * Owns a self-contained set of GPU buffers parallel to GPUSceneSystem so foliage
 * instance churn never touches the scene path:
 *   - instance buffer     : compact FoliageInstanceData pool
 *   - header buffer        : live instance count (for the cull dispatch)
 *   - indirect buffer      : VkDrawIndexedIndirectCommand produced by the cull pass
 *   - count buffer         : single uint, atomically incremented by the cull pass
 *   - per-draw buffer      : FoliagePerDrawData consumed by the foliage VS/PS
 *
 * For now it generates a cross-quad "grass" mesh and a hardcoded grid of instances
 * to prove the cull -> indirect-draw path. Density-map driven scatter replaces the
 * hardcoded grid in a later phase.
 */
class FoliageSystem final : public System
{
public:
    FoliageSystem();
    ~FoliageSystem() override;

    void Init() override;

    uint GetNumInstances() const { return myNumInstances; }
    uint GetCapacity() const { return myCapacity; }

    VulkanBuffer* GetIndirectBuffer() const { return myIndirectBuffer; }
    VulkanBuffer* GetCountBuffer() const { return myCountBuffer; }

private:
    void CreateBuffers();
    void CreateFoliageMesh();
    void GenerateTestInstances();

    static constexpr uint myCapacity = 4096; // max instances supported in phase 1

    Mesh* myMesh = nullptr;
    VertexBufferHandle* myVertexBuffer = nullptr;
    IndexBufferHandle* myIndexBuffer = nullptr;

    VulkanBuffer* myInstanceBuffer = nullptr;
    VulkanBuffer* myHeaderBuffer = nullptr;
    VulkanBuffer* myIndirectBuffer = nullptr;
    VulkanBuffer* myCountBuffer = nullptr;
    VulkanBuffer* myPerDrawBuffer = nullptr;

    uint myNumInstances = 0;
};
