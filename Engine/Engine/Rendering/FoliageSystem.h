#pragma once
#include "Engine/Shaders/Shared/FoliageStructs.hpp"
#include "Engine/System/System.h"

class Mesh;
class VulkanBuffer;
class VertexBufferHandle;
class IndexBufferHandle;

/*
 * A kind of foliage: which mesh to scatter, how densely, and how it varies.
 * Phase 2 keeps this a runtime config; it becomes a serialized ".foliage" asset
 * (with a real material) when the editor painting tools land.
 */
struct FoliageType
{
    Mesh* myMesh = nullptr;
    VertexBufferHandle* myVertexBuffer = nullptr;
    IndexBufferHandle* myIndexBuffer = nullptr;

    glm::vec3 myTint = glm::vec3(1.0f);   // base/fallback colour (no material yet)
    float myMinScale = 1.0f;
    float myMaxScale = 1.0f;
    int myInstancesPerAxis = 32;          // density: grid resolution over the field
};

/*
 * Phase 1+2 of the GPU-driven foliage system (see docs/foliage-rendering-plan.md).
 *
 * Owns a self-contained set of GPU buffers parallel to GPUSceneSystem so foliage
 * instance churn never touches the scene path:
 *   - instance buffer  : compact FoliageInstanceData pool
 *   - header buffer    : live instance count (for the cull dispatch)
 *   - indirect buffer  : VkDrawIndexedIndirectCommand produced by the cull pass
 *   - count buffer     : single uint, atomically incremented by the cull pass
 *   - per-draw buffer  : FoliagePerDrawData consumed by the foliage VS/PS
 *
 * Renders multiple FoliageTypes from a hardcoded per-type scatter grid. Density-map
 * driven scatter and editor painting replace the hardcoded grid in a later phase.
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
    void CreateFoliageTypes();
    void GenerateInstances();

    // Builds a double-sided "cross" of two perpendicular quads, sized width x height.
    void CreateCrossQuadMesh(float inWidth, float inHeight, FoliageType& outType);

    static constexpr uint myCapacity = 16384; // max instances supported

    List<FoliageType> myTypes;

    VulkanBuffer* myInstanceBuffer = nullptr;
    VulkanBuffer* myHeaderBuffer = nullptr;
    VulkanBuffer* myIndirectBuffer = nullptr;
    VulkanBuffer* myCountBuffer = nullptr;
    VulkanBuffer* myPerDrawBuffer = nullptr;

    uint myNumInstances = 0;
};
