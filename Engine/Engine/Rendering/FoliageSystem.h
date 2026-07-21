#pragma once
#include "Engine/Math/Heightfield.h"
#include "Engine/Shaders/Shared/FoliageStructs.hpp"
#include "Engine/System/System.h"

class Mesh;
class VulkanBuffer;
class VertexBufferHandle;
class IndexBufferHandle;

/*
 * A kind of foliage: which mesh to scatter, how it varies, and a painted density
 * map controlling where it grows. Phase 2/A keeps this a runtime config; it becomes
 * a serialized ".foliage" asset (with a real material + LOD list) later.
 */
struct FoliageType
{
    std::string myName;

    Mesh* myMesh = nullptr;
    VertexBufferHandle* myVertexBuffer = nullptr;
    IndexBufferHandle* myIndexBuffer = nullptr;

    glm::vec3 myTint = glm::vec3(1.0f);   // base/fallback colour (no material yet)
    float myMinScale = 1.0f;
    float myMaxScale = 1.0f;

    // Probability multiplier applied to the density map value when scattering. >1 packs
    // more instances into painted areas, <1 thins them.
    float myDensityMultiplier = 1.0f;

    // Row-major density grid over the field, values in [0,1]. Painted by the editor.
    List<float> myDensityMap;
};

/*
 * GPU-driven foliage (see docs/foliage-rendering-plan.md).
 *
 * Placement is generated on the CPU from per-type density maps and the procedural
 * heightfield (so foliage conforms to the terrain), then uploaded to a compact
 * instance pool. Rendering is fully GPU-driven: FoliageCullPass (compute) builds
 * indirect draws consumed by FoliagePass. The instance/indirect/count/per-draw
 * buffers are owned here, parallel to GPUSceneSystem.
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

    // ---- Editor-facing authoring API ----
    int GetTypeCount() const { return myTypes.size(); }
    const std::string& GetTypeName(int inTypeIndex) const { return myTypes[inTypeIndex].myName; }
    const glm::vec3& GetTypeTint(int inTypeIndex) const { return myTypes[inTypeIndex].myTint; }

    static constexpr int myDensityRes = 96;     // density-map resolution per type
    static constexpr float myFieldSize = 6000.0f; // world extent the field covers (centred on origin)

    // Add/subtract density of one type inside a world-space brush, then regenerate.
    void PaintDensity(int inTypeIndex, const glm::vec2& inWorldXZ, float inWorldRadius, float inStrength, bool inAdditive);

    // March a ray against the procedural heightfield. Returns false if it never hits.
    bool RaycastHeightfield(const glm::vec3& inOrigin, const glm::vec3& inDirection, glm::vec3& outHitPos) const;

    // Fill one type's density map with a uniform value (used by the editor's fill/clear).
    void SetTypeDensity(int inTypeIndex, float inValue);

    // Rebuild the instance pool from the current density maps + heightfield and upload it.
    void RegenerateInstances();

    // Push runtime scalability knobs to the GPU cull pass.
    void SetScalability(float inMaxDistanceScale, float inDensityScale);

private:
    void CreateBuffers();
    void CreateFoliageTypes();

    // Builds a double-sided "cross" of two perpendicular quads, sized width x height.
    void CreateCrossQuadMesh(float inWidth, float inHeight, FoliageType& outType);

    glm::vec2 CellToWorld(int inCellX, int inCellZ) const;

    static constexpr uint myCapacity = 16384; // max instances supported

    Heightfield myHeightfield{};
    List<FoliageType> myTypes;

    VulkanBuffer* myInstanceBuffer = nullptr;
    VulkanBuffer* myHeaderBuffer = nullptr;
    VulkanBuffer* myIndirectBuffer = nullptr;   // FOLIAGE_LOD_COUNT indirect draw commands (one per LOD bin)
    VulkanBuffer* myBladeBuffer = nullptr;      // FOLIAGE_LOD_COUNT * myCapacity blade records
    VulkanBuffer* myScalabilityBuffer = nullptr;

    FoliageScalabilitySettings mySettings{};

    uint myNumInstances = 0;
};
