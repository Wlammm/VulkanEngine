// GPU-driven foliage culling. One thread per instance: reconstruct the world
// matrix from the compact instance record and emit an indirect draw command plus
// per-draw payload. Mirrors IndirectCullingCS but writes the foliage-only buffers.
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

// Build a translate * rotateY * scale matrix for use with mul(M, position). The
// foliage path produces and consumes this matrix entirely on its own, so the
// convention only needs to be internally consistent with FoliageVS.
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

[numthreads(256, 1, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint instanceIndex = dispatchThreadID.x;
    if (instanceIndex >= inFoliageHeader.myNumInstances)
        return;

    FoliageInstanceData instance = inFoliageInstances[instanceIndex];

    MeshData meshData = inMeshBuffer[instance.myMeshIndex];
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
    outFoliagePerDrawData[renderIndex].myPadding        = 0;
}
