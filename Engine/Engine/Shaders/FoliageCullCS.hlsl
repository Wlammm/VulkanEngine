// GPU-driven foliage culling. One thread per instance: frustum + distance cull,
// distance-based density fade, then (for survivors) reconstruct the world matrix and
// emit an indirect draw command plus per-draw payload. Mirrors IndirectCullingCS.
// See docs/foliage-rendering-plan.md.
#include "Shared/MeshStructs.hpp"
#include "Shared/FoliageStructs.hpp"

// -------------------- Input Buffers --------------------
[[vk::binding(0)]] StructuredBuffer<FoliageInstanceData> inFoliageInstances;
[[vk::binding(1)]] StructuredBuffer<MeshData> inMeshBuffer;
[[vk::binding(2)]] StructuredBuffer<VertexBufferData> inVertexDataBuffer;
[[vk::binding(3)]] StructuredBuffer<IndexBufferData> inIndexDataBuffer;

// -------------------- Output Buffers --------------------
[[vk::binding(4)]] RWStructuredBuffer<FoliageDrawCommand> outFoliageIndirectBuffer;
[[vk::binding(5)]] RWStructuredBuffer<uint> outFoliageCountBuffer;
[[vk::binding(6)]] RWStructuredBuffer<FoliagePerDrawData> outFoliagePerDrawData;

[[vk::binding(7)]] ConstantBuffer<FoliageSceneHeader> inFoliageHeader : register(b0);
[[vk::binding(8)]] ConstantBuffer<CameraBuffer> inCameraBuffer : register(b1);
[[vk::binding(9)]] ConstantBuffer<FoliageScalabilitySettings> inFoliageScalability : register(b2);

float4x4 BuildWorldMatrix(float3 inPosition, float inScale, float inYaw)
{
    float s = sin(inYaw);
    float c = cos(inYaw);

    return float4x4(
        c * inScale, 0.0,     s * inScale, inPosition.x,
        0.0,         inScale, 0.0,         inPosition.y,
       -s * inScale, 0.0,     c * inScale, inPosition.z,
        0.0,         0.0,     0.0,         1.0);
}

// Stable per-instance pseudo-random in [0,1), used for distance density fade.
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

    MeshData meshData = inMeshBuffer[instance.myMeshIndex];

    // Conservative world-space bounding sphere (centre at the instance origin so we
    // can ignore rotation): radius covers the model sphere offset + radius, scaled.
    float modelRadius = length(meshData.myBoundingSphereModelSpace.xyz) + meshData.myBoundingSphereModelSpace.w;
    float worldRadius = modelRadius * instance.myScale;
    float3 worldCenter = instance.myPosition;

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

    // Survivor: emit the draw.
    VertexBufferData vertexData = inVertexDataBuffer[meshData.myVertexIndex];
    IndexBufferData indexData = inIndexDataBuffer[meshData.myIndexDataIndex];

    uint renderIndex;
    InterlockedAdd(outFoliageCountBuffer[0], 1, renderIndex);

    outFoliageIndirectBuffer[renderIndex].indexCount    = indexData.myCount;
    outFoliageIndirectBuffer[renderIndex].instanceCount = 1;
    outFoliageIndirectBuffer[renderIndex].firstIndex    = indexData.myOffset;
    outFoliageIndirectBuffer[renderIndex].vertexOffset  = (int)(vertexData.myByteOffset / VERTEX_STRIDE_BYTES);
    outFoliageIndirectBuffer[renderIndex].firstInstance = 0;

    outFoliagePerDrawData[renderIndex].myToWorld        = BuildWorldMatrix(instance.myPosition, instance.myScale, instance.myRotationY);
    outFoliagePerDrawData[renderIndex].myAlbedoIndex    = instance.myAlbedoIndex;
    outFoliagePerDrawData[renderIndex].myNormalIndex    = instance.myNormalIndex;
    outFoliagePerDrawData[renderIndex].myMaterialIndex  = instance.myMaterialIndex;
    outFoliagePerDrawData[renderIndex].myTintPacked     = instance.myTintPacked;
}
