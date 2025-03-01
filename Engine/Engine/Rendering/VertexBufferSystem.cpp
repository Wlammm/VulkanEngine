#include "EnginePch.h"
#include "VertexBufferSystem.h"

#include "Engine.h"
#include "RenderSystem.h"
#include "VertexBufferHandle.h"
#include "Utils/MathUtils.hpp"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"

VertexBufferSystem::VertexBufferSystem()
{
    myBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("Global Vertex Buffer",
        vk::BufferCreateInfo()
        .setSize(sizeof(Vertex) * 16)
        .setUsage(vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst| vk::BufferUsageFlagBits::eTransferSrc)
        , VMA_MEMORY_USAGE_AUTO,
        false));

    mySparseVertexDataBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("Global Sparse Vertex Data Buffer",
      vk::BufferCreateInfo()
      .setSize(sizeof(VertexBufferData) * 16)
      .setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst| vk::BufferUsageFlagBits::eTransferSrc)
      , VMA_MEMORY_USAGE_AUTO,
      false));

}

VertexBufferSystem::~VertexBufferSystem()
{
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
    VulkanAllocator::DestroyBuffer_TS(mySparseVertexDataBuffer);
    myUsedBufferSize = 0;

    for(VertexBufferHandle* vertexBuffer : myVertexBuffers)
    {
        del(vertexBuffer);
    }
    myVertexBuffers.Clear();
}

VertexBufferHandle* VertexBufferSystem::UploadVertexBuffer(VulkanBuffer* inStagingBuffer, const uint inVertexCount)
{
    check(inVertexCount > 0);

    // Updload the new data to the vertex data buffer.
    uint dataIndex = -1;
    if(!myFreeSparseIndices.IsEmpty())
    {
        dataIndex = myFreeSparseIndices.Last();
        myFreeSparseIndices.RemoveLast();
    }
    else
    {
        dataIndex = mySparseVertexData_CPURepresentation.size();
        mySparseVertexData_CPURepresentation.Emplace();
    }
    
    VertexBufferData& data = mySparseVertexData_CPURepresentation[dataIndex];
    data.myOffset = myCurrentVertexOffset;

    // Upload the new vertices.
    const uint sizeIncrease = inVertexCount * sizeof(Vertex);
    const uint requiredSize = myUsedBufferSize + sizeIncrease;
    
    myBuffer->CopyDataFromBuffer(inStagingBuffer, sizeIncrease, myUsedBufferSize);
    mySparseVertexDataBuffer->SetData(&data, sizeof(VertexBufferData), sizeof(VertexBufferData) * dataIndex);
    
    myUsedBufferSize += sizeIncrease;
    myCurrentVertexOffset += inVertexCount;

    VertexBufferHandle* buffer = new VertexBufferHandle();
    myVertexBuffers.Add(buffer);
    buffer->myIndex = dataIndex;
    
    return buffer;
}

VertexBufferHandle* VertexBufferSystem::UploadVertexBuffer(const List<Vertex>& inVertices)
{
    ZoneScoped;
    check(false && "Not sure this is working with the new sparse buffer system. Check and fix before using.");

    // Upload the new data to the vertex data buffer.
    uint dataIndex = -1;
    if(!myFreeSparseIndices.IsEmpty())
    {
        dataIndex = myFreeSparseIndices.Last();
        myFreeSparseIndices.RemoveLast();
    }
    else
    {
        dataIndex = mySparseVertexData_CPURepresentation.size();
        mySparseVertexData_CPURepresentation.Emplace();
    }
    
    VertexBufferData& data = mySparseVertexData_CPURepresentation[dataIndex];
    data.myOffset = myCurrentVertexOffset;
    
    const uint sizeIncrease = inVertices.size() * sizeof(Vertex);
    const uint requiredSize = myUsedBufferSize + sizeIncrease;
    myBuffer->SetData(inVertices.data(), sizeIncrease, myUsedBufferSize);

    myUsedBufferSize += sizeIncrease;
    myCurrentVertexOffset += static_cast<uint>(inVertices.size());

    VertexBufferHandle* buffer = new VertexBufferHandle();
    myVertexBuffers.Add(buffer);
    buffer->myIndex = dataIndex;
    return buffer;
}

void VertexBufferSystem::RemoveVertexBuffer(const VertexBufferHandle* inBuffer)
{
    LOG_WARNING("VertexBufferSubsystem::RemoveVertexBuffer not implemented.");
}

const ResizableBuffer* VertexBufferSystem::GetGlobalVertexBuffer() const
{
    return myBuffer;
}

const ResizableBuffer* VertexBufferSystem::GetGlobalSparseVertexDataBuffer() const
{
    return mySparseVertexDataBuffer;
}

uint VertexBufferSystem::GetUsedBufferSize() const
{
    return myUsedBufferSize;    
}

uint VertexBufferSystem::GetVertexOffsetFromVertexHandle(VertexBufferHandle* inBuffer) const
{
    return mySparseVertexData_CPURepresentation[inBuffer->myIndex].myOffset;
}
