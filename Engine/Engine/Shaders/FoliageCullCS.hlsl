// GPU-driven foliage culling. One thread per instance: frustum + distance cull and
// distance-based density fade, then (for survivors) pick a LOD by distance, derive a
// compact per-blade record, and append it into that LOD's bin while bumping the bin's
// indirect draw count. Blade geometry itself is generated procedurally in FoliageVS.
// See docs/foliage-rendering-plan.md.
#include "Shared/MeshStructs.hpp"
#include "Shared/FoliageStructs.hpp"

// -------------------- Input Buffers --------------------
[[vk::binding(0)]] StructuredBuffer<FoliageInstanceData> inFoliageInstances;

// -------------------- Output Buffers --------------------
// One indirect command per LOD bin (instanceCount accumulated here via InterlockedAdd).
[[vk::binding(4)]] RWStructuredBuffer<FoliageDrawCommand> outFoliageIndirectBuffer;
// Per-blade records, laid out as FOLIAGE_LOD_COUNT contiguous regions of myBinCapacity each.
[[vk::binding(6)]] RWStructuredBuffer<FoliageBladeData> outFoliageBladeBuffer;

[[vk::binding(7)]] ConstantBuffer<FoliageSceneHeader> inFoliageHeader : register(b0);
[[vk::binding(8)]] ConstantBuffer<CameraBuffer> inCameraBuffer : register(b1);
[[vk::binding(9)]] ConstantBuffer<FoliageScalabilitySettings> inFoliageScalability : register(b2);

// Stable per-instance pseudo-random in [0,1), used for density fade and blade variation.
float Hash01(uint inSeed)
{
    inSeed ^= inSeed >> 16;
    inSeed *= 0x7feb352du;
    inSeed ^= inSeed >> 15;
    inSeed *= 0x846ca68bu;
    inSeed ^= inSeed >> 16;
    return float(inSeed & 0xFFFFFFu) / float(0x1000000u);
}

// Conservative frustum test of a world-space bounding sphere against the 6 planes of
// viewProj (rows give the planes for the mul(M, v) convention the engine uses).
bool SphereInFrustum(float4x4 inViewProj, float3 inCenter, float inRadius)
{
    float4 planes[6];
    planes[0] = inViewProj[3] + inViewProj[0]; // left
    planes[1] = inViewProj[3] - inViewProj[0]; // right
    planes[2] = inViewProj[3] + inViewProj[1]; // bottom
    planes[3] = inViewProj[3] - inViewProj[1]; // top
    planes[4] = inViewProj[2];                 // near (Vulkan z in [0,1])
    planes[5] = inViewProj[3] - inViewProj[2]; // far

    for (int i = 0; i < 6; ++i)
    {
        float len = length(planes[i].xyz);
        if (len < 1e-6)
            continue;

        float signedDist = (dot(planes[i].xyz, inCenter) + planes[i].w) / len;
        if (signedDist < -inRadius)
            return false;
    }
    return true;
}

[numthreads(256, 1, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint instanceIndex = dispatchThreadID.x;
    if (instanceIndex >= inFoliageHeader.myNumInstances)
        return;

    FoliageInstanceData instance = inFoliageInstances[instanceIndex];

    // The per-instance scale is the blade height in world units.
    float height = instance.myScale;
    float3 rootPos = instance.myPosition;

    // Conservative bounding sphere around the blade (centre at mid-height).
    float3 worldCenter = rootPos + float3(0.0, height * 0.5, 0.0);
    float worldRadius = height;

    // Frustum cull.
    float4x4 viewProj = mul(inCameraBuffer.myProjection, inCameraBuffer.myToView);
    if (!SphereInFrustum(viewProj, worldCenter, worldRadius))
        return;

    // Distance cull + density fade.
    float distanceToCamera = length(inCameraBuffer.myCameraPosition - worldCenter);
    float distanceScale = inFoliageScalability.myGlobalMaxDistanceScale;
    float maxDistance = inFoliageScalability.myMaxDrawDistance * distanceScale;
    float fadeStart = inFoliageScalability.myFadeStartDistance * distanceScale;

    if (distanceToCamera > maxDistance + worldRadius)
        return;

    float fade = saturate((distanceToCamera - fadeStart) / max(maxDistance - fadeStart, 1.0));
    float keepProbability = (1.0 - fade) * inFoliageScalability.myGlobalDensityScale;
    if (Hash01(instanceIndex * 2654435761u) > keepProbability)
        return;

    // LOD select by distance as a fraction of the max draw distance (near -> LOD0).
    float distFrac = distanceToCamera / max(maxDistance, 1.0);
    uint lod = 0;
    if (distFrac > 0.5)
        lod = 2;
    else if (distFrac > 0.2)
        lod = 1;

    // Per-blade randomised shaping so a field of identical instances still varies.
    float r0 = Hash01(instanceIndex * 747796405u + 1u);
    float r1 = Hash01(instanceIndex * 2891336453u + 2u);

    FoliageBladeData blade;
    blade.myRootPosition = rootPos;
    blade.myHeight = height;
    blade.myWidth = height * 0.08;
    blade.myBend = height * (0.15 + 0.35 * r1);
    blade.myYaw = instance.myRotationY + (r0 - 0.5) * 0.5;
    blade.myTintPacked = instance.myTintPacked;

    // Append into this LOD's bin and bump its instanceCount.
    uint slot;
    InterlockedAdd(outFoliageIndirectBuffer[lod].instanceCount, 1, slot);
    if (slot >= inFoliageHeader.myBinCapacity)
        return; // bin full this frame, drop the blade

    outFoliageBladeBuffer[lod * inFoliageHeader.myBinCapacity + slot] = blade;
}
