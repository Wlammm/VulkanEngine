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

// ---- No-depth variants ----

[[vk::binding(8)]]
RWStructuredBuffer<PerDrawData> outPerDrawDataNoDepth;

[[vk::binding(9)]]
RWStructuredBuffer<uint> outCountBufferNoDepth;

[[vk::binding(10)]]
RWStructuredBuffer<DrawIndexedIndirectCommand> outIndirectBufferNoDepth;

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

    bool depthWriteEnabled = (instanceData.myDrawFlags & DrawFlag_NoDepthTest) == 0;
    
    if (depthWriteEnabled)
    {
        uint renderIndex;
        InterlockedAdd(outCountBuffer[0], 1, renderIndex);
        
        outIndirectBuffer[renderIndex].firstInstance = 0;
        outIndirectBuffer[renderIndex].instanceCount = 1;
        outIndirectBuffer[renderIndex].indexCount    = indexData.myCount;
        outIndirectBuffer[renderIndex].vertexOffset = (int)vertexData.myOffset;
        outIndirectBuffer[renderIndex].firstIndex   = indexData.myOffset;
        
        outPerDrawData[renderIndex].myAlbedoIndex   = instanceData.myAlbedoIndex;
        outPerDrawData[renderIndex].myNormalIndex   = instanceData.myNormalIndex;
        outPerDrawData[renderIndex].myMaterialIndex = instanceData.myMaterialIndex;
        outPerDrawData[renderIndex].myToWorld       = instanceData.myToWorld;
    }
    else
    {
        uint renderIndex;
        InterlockedAdd(outCountBufferNoDepth[0], 1, renderIndex);
        
        outIndirectBufferNoDepth[renderIndex].firstInstance = 0;
        outIndirectBufferNoDepth[renderIndex].instanceCount = 1;
        outIndirectBufferNoDepth[renderIndex].indexCount    = indexData.myCount;
        outIndirectBufferNoDepth[renderIndex].vertexOffset = (int)vertexData.myOffset;
        outIndirectBufferNoDepth[renderIndex].firstIndex   = indexData.myOffset;
        
        outPerDrawDataNoDepth[renderIndex].myAlbedoIndex   = instanceData.myAlbedoIndex;
        outPerDrawDataNoDepth[renderIndex].myNormalIndex   = instanceData.myNormalIndex;
        outPerDrawDataNoDepth[renderIndex].myMaterialIndex = instanceData.myMaterialIndex;
        outPerDrawDataNoDepth[renderIndex].myToWorld       = instanceData.myToWorld;
    }
}
