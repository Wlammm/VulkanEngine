// Includes
#include "Shared/MeshStructs.hpp"
#include "VulkanStructs.hlsli"

// -------------------- Input Buffers --------------------
[[vk::binding(0)]]
StructuredBuffer<MeshData> inMeshBuffer;

[[vk::binding(6)]]
StructuredBuffer<VertexBufferData> inVertexDataBuffer;

[[vk::binding(7)]]
StructuredBuffer<IndexBufferData> inIndexDataBuffer;


[[vk::binding(1)]]
StructuredBuffer<MeshInstanceData> inMeshInstances;

// -------------------- Output Buffers --------------------
[[vk::binding(2)]]
RWStructuredBuffer<DrawIndexedIndirectCommand> outIndirectBuffer;

[[vk::binding(3)]]
RWStructuredBuffer<uint> outCountBuffer;

[[vk::binding(4)]]
RWStructuredBuffer<PerDrawData> outPerDrawData;

[[vk::binding(11)]]
ConstantBuffer<SceneHeader> inSceneHeader : register(b0);

// -------------------- Compute Shader --------------------

[numthreads(256, 1, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint meshInstanceIndex = dispatchThreadID.x;
    
    if (meshInstanceIndex >= inSceneHeader.myNumMeshInstances)
        return;
    
    MeshInstanceData instanceData = inMeshInstances[meshInstanceIndex];
    
    MeshData meshData = inMeshBuffer[instanceData.myMeshIndex];
    
    VertexBufferData vertexData = inVertexDataBuffer[meshData.myVertexIndex];
    
    IndexBufferData indexData = inIndexDataBuffer[meshData.myIndexDataIndex];

    for (int shadingBinIndex = 0; shadingBinIndex < ShadingBin_Count; ++shadingBinIndex)
    {
        if (ShouldDrawForShadingBin((EDrawFlags)instanceData.myDrawFlags, (EShadingBin)shadingBinIndex))
        {
            uint renderIndex;
            // Get an index from the bins count buffer, and multiply by the shading bin index to offset to the correct indirect parts.
            InterlockedAdd(outCountBuffer[shadingBinIndex], 1, renderIndex);
            
            // myNumMeshInstances should always match the elements in each shading bin. Otherwise we need to update that later on.
            renderIndex += shadingBinIndex * inSceneHeader.myNumMeshInstances;
        
            outIndirectBuffer[renderIndex].firstInstance = 0;
            outIndirectBuffer[renderIndex].instanceCount = 1;
            outIndirectBuffer[renderIndex].indexCount    = indexData.myCount;
            outIndirectBuffer[renderIndex].vertexOffset = (int)(vertexData.myByteOffset / VERTEX_STRIDE_BYTES);
            outIndirectBuffer[renderIndex].firstIndex   = indexData.myOffset;
        
            outPerDrawData[renderIndex].myAlbedoIndex   = instanceData.myAlbedoIndex;
            outPerDrawData[renderIndex].myNormalIndex   = instanceData.myNormalIndex;
            outPerDrawData[renderIndex].myMaterialIndex = instanceData.myMaterialIndex;
            outPerDrawData[renderIndex].myToWorld       = instanceData.myToWorld;
        }
    }
}
