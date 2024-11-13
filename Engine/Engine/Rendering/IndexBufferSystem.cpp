#include "EnginePch.h"
#include "IndexBufferSystem.h"

#include "Engine.h"
#include "IndexBufferHandle.h"
#include "RenderSystem.h"
#include "Utils/MathUtils.hpp"
#include "Vulkan/ResizableBuffer.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"

IndexBufferSystem::IndexBufferSystem()
{
    myBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("Global IndexBuffer", vk::BufferCreateInfo()
        .setSize(MathUtils::UpperPowerOfTwo(sizeof(uint) * 4))
        .setUsage(vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc), VMA_MEMORY_USAGE_AUTO, false));
}

IndexBufferSystem::~IndexBufferSystem()
{
    myUsedBufferSize = 0;
    myCurrentIndexOffset = 0;

    for(IndexBufferHandle* indexBuffer : myIndexBuffers)
    {
        del(indexBuffer);
    }
    myIndexBuffers.Clear();
    
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
}

IndexBufferHandle* IndexBufferSystem::UploadIndexBuffer(const List<uint>& inIndices)
{
    ZoneScoped;
    IndexBufferHandle* buffer = new IndexBufferHandle();
    const uint sizeIncrease = inIndices.size() * sizeof(uint);

    myBuffer->SetData(inIndices.data(), sizeIncrease, myUsedBufferSize);
    
    buffer->myOffset = myCurrentIndexOffset;
    buffer->myIndexCount = inIndices.size();
    myIndexBuffers.Add(buffer);
    
    myUsedBufferSize += sizeIncrease;
    myCurrentIndexOffset += static_cast<uint>(inIndices.size());
    return buffer;
}

IndexBufferHandle* IndexBufferSystem::UploadIndexBuffer(VulkanBuffer* inStagingBuffer, const uint inVertexCount)
{
    ZoneScoped;
    IndexBufferHandle* buffer = new IndexBufferHandle();
    const uint sizeIncrease = inVertexCount * sizeof(uint);

    myBuffer->CopyDataFromBuffer(inStagingBuffer, sizeIncrease, myUsedBufferSize);
    
    buffer->myOffset = myCurrentIndexOffset;
    buffer->myIndexCount = inVertexCount;
    myIndexBuffers.Add(buffer);
    
    myUsedBufferSize += sizeIncrease;
    myCurrentIndexOffset += static_cast<uint>(inVertexCount);
    return buffer;
}

void IndexBufferSystem::RemoveIndexBuffer(const IndexBufferHandle* inBuffer)
{
    LOG_WARNING("IndexBufferSubsystem::RemoveIndexBuffer not implemented.");
}

const ResizableBuffer* IndexBufferSystem::GetGlobalIndexBuffer() const
{
    return myBuffer;
}