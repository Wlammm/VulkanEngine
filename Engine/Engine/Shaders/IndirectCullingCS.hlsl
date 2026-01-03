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
StructuredBuffer<MeshInstanceData> inSparseObjectBuffer;

struct DenseObjectBuffer
{
    uint myNumObjects;
    uint myIndices[999999];
};

[[vk::binding(5)]]
StructuredBuffer<DenseObjectBuffer> inDenseObjectBuffer;

// -------------------- Output Buffers --------------------

struct OutIndirectBuffer
{
    DrawIndexedIndirectCommand drawCommands[9999999];
};

[[vk::binding(2)]]
RWStructuredBuffer<OutIndirectBuffer> outIndirectBuffer;

struct OutCountBuffer
{
    uint myDrawCount;
};

[[vk::binding(3)]]
RWStructuredBuffer<OutCountBuffer> outCountBuffer;

struct OutPerDrawDataBuffer
{
    PerDrawData perDrawData[9999999];
};

[[vk::binding(4)]]
RWStructuredBuffer<OutPerDrawDataBuffer> outPerDrawData;

// ---- No-depth variants ----

[[vk::binding(8)]]
RWStructuredBuffer<OutPerDrawDataBuffer> outPerDrawDataNoDepth;

[[vk::binding(9)]]
RWStructuredBuffer<OutCountBuffer> outCountBufferNoDepth;

[[vk::binding(10)]]
RWStructuredBuffer<OutIndirectBuffer> outIndirectBufferNoDepth;

// -------------------- Compute Shader --------------------

[numthreads(256, 1, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint denseIndex = dispatchThreadID.x;
    
    if (denseIndex >= inDenseObjectBuffer[0].myNumObjects)
        return;
    
    uint sparseIndex = inDenseObjectBuffer[0].myIndices[denseIndex];
    
    MeshInstanceData instanceData = inSparseObjectBuffer[sparseIndex];
    
    MeshData meshData = inMeshBuffer[instanceData.myMeshIndex];
    
    VertexBufferData vertexData = inVertexDataBuffer[meshData.myVertexIndex];
    
    IndexBufferData indexData = inIndexDataBuffer[meshData.myIndexDataIndex];

    bool depthWriteEnabled = instanceData.myDepthWriteEnabled == 1;
    
    if (depthWriteEnabled)
    {
        uint renderIndex;
        InterlockedAdd(outCountBuffer[0].myDrawCount, 1, renderIndex);
        
        outIndirectBuffer[0].drawCommands[renderIndex].firstInstance = 0;
        outIndirectBuffer[0].drawCommands[renderIndex].instanceCount = 1;
        outIndirectBuffer[0].drawCommands[renderIndex].indexCount    = indexData.myCount;
        outIndirectBuffer[0].drawCommands[renderIndex].vertexOffset = (int)vertexData.myOffset;
        outIndirectBuffer[0].drawCommands[renderIndex].firstIndex   = indexData.myOffset;
        
        outPerDrawData[0].perDrawData[renderIndex].myAlbedoIndex   = instanceData.myAlbedoIndex;
        outPerDrawData[0].perDrawData[renderIndex].myNormalIndex   = instanceData.myNormalIndex;
        outPerDrawData[0].perDrawData[renderIndex].myMaterialIndex = instanceData.myMaterialIndex;
        outPerDrawData[0].perDrawData[renderIndex].myToWorld       = instanceData.myToWorld;
    }
    else
    {
        uint renderIndex;
        InterlockedAdd(outCountBufferNoDepth[0].myDrawCount, 1, renderIndex);
        
        outIndirectBufferNoDepth[0].drawCommands[renderIndex].firstInstance = 0;
        outIndirectBufferNoDepth[0].drawCommands[renderIndex].instanceCount = 1;
        outIndirectBufferNoDepth[0].drawCommands[renderIndex].indexCount    = indexData.myCount;
        outIndirectBufferNoDepth[0].drawCommands[renderIndex].vertexOffset = (int)vertexData.myOffset;
        outIndirectBufferNoDepth[0].drawCommands[renderIndex].firstIndex   = indexData.myOffset;
        
        outPerDrawDataNoDepth[0].perDrawData[renderIndex].myAlbedoIndex   = instanceData.myAlbedoIndex;
        outPerDrawDataNoDepth[0].perDrawData[renderIndex].myNormalIndex   = instanceData.myNormalIndex;
        outPerDrawDataNoDepth[0].perDrawData[renderIndex].myMaterialIndex = instanceData.myMaterialIndex;
        outPerDrawDataNoDepth[0].perDrawData[renderIndex].myToWorld       = instanceData.myToWorld;
    }
}
