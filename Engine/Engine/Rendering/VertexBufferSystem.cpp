#include "EnginePch.h"
#include "VertexBufferSystem.h"

#include "Engine.h"
#include "RenderSystem.h"
#include "VertexBuffer.h"
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
}

VertexBufferSystem::~VertexBufferSystem()
{
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
    myUsedBufferSize = 0;

    for(VertexBuffer* vertexBuffer : myVertexBuffers)
    {
        del(vertexBuffer);
    }
    myVertexBuffers.Clear();
}

VertexBuffer* VertexBufferSystem::UploadVertexBuffer(VulkanBuffer* inStagingBuffer, const uint inVertexCount)
{
    check(inVertexCount > 0);
    VertexBuffer* buffer = new VertexBuffer();
    const uint sizeIncrease = inVertexCount * sizeof(Vertex);
    const uint requiredSize = myUsedBufferSize + sizeIncrease;

    uint dataIndex = -1;
    if(!myFreeVertexBufferIndices.empty())
    {
        dataIndex = myFreeVertexBufferIndices.back();
        myFreeVertexBufferIndices.pop();
    }
    else
    {
        dataIndex = myNextVertexBufferIndex++;
    }
    
    myBuffer->CopyDataFromBuffer(inStagingBuffer, sizeIncrease, myUsedBufferSize);
    buffer->myIndex = dataIndex;
    //buffer->myOffset = myCurrentVertexOffset;
    //buffer->myVertexCount = inVertexCount;
    myVertexBuffers.Add(buffer);

    myUsedBufferSize += sizeIncrease;
    myCurrentVertexOffset += inVertexCount;
    return buffer;
}

VertexBuffer* VertexBufferSystem::UploadVertexBuffer(const List<Vertex>& inVertices)
{
    ZoneScoped;
    VertexBuffer* buffer = new VertexBuffer();
    const uint sizeIncrease = inVertices.size() * sizeof(Vertex);
    const uint requiredSize = myUsedBufferSize + sizeIncrease;
    myBuffer->SetData(inVertices.data(), sizeIncrease, myUsedBufferSize);

    buffer->myOffset = myCurrentVertexOffset;
    buffer->myVertexCount = inVertices.size();
    myVertexBuffers.Add(buffer);
    
    myUsedBufferSize += sizeIncrease;
    myCurrentVertexOffset += static_cast<uint>(inVertices.size());
    return buffer;
}

void VertexBufferSystem::RemoveVertexBuffer(const VertexBuffer* inBuffer)
{
    LOG_WARNING("VertexBufferSubsystem::RemoveVertexBuffer not implemented.");
}

const ResizableBuffer* VertexBufferSystem::GetGlobalVertexBuffer() const
{
    return myBuffer;
}

uint VertexBufferSystem::GetUsedBufferSize() const
{
    return myUsedBufferSize;    
}
