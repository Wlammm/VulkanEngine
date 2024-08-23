#include "EnginePch.h"
#include "IndexBufferSystem.h"

#include "Engine.h"
#include "IndexBuffer.h"
#include "RenderSystem.h"
#include "Utils/MathUtils.hpp"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"

IndexBufferSystem::IndexBufferSystem()
{
    GrowBuffer(sizeof(uint) * 4);    
}

IndexBufferSystem::~IndexBufferSystem()
{
    myUsedBufferSize = 0;
    myCurrentIndexOffset = 0;

    for(IndexBuffer* indexBuffer : myIndexBuffers)
    {
        del(indexBuffer);
    }
    myIndexBuffers.Clear();
    
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
}

IndexBuffer* IndexBufferSystem::UploadIndexBuffer(const List<uint>& inIndices)
{
    ZoneScoped;
    IndexBuffer* buffer = new IndexBuffer();
    const uint sizeIncrease = inIndices.size() * sizeof(uint);
    const uint requiredSize = myUsedBufferSize + sizeIncrease;
    GrowBuffer(requiredSize);

    myBuffer->SetData(inIndices.data(), sizeIncrease, myUsedBufferSize);
    
    buffer->myOffset = myCurrentIndexOffset;
    buffer->myIndexCount = inIndices.size();
    myIndexBuffers.Add(buffer);
    
    myUsedBufferSize += sizeIncrease;
    myCurrentIndexOffset += static_cast<uint>(inIndices.size());
    return buffer;
}

void IndexBufferSystem::RemoveIndexBuffer(const IndexBuffer* inBuffer)
{
    LOG_WARNING("IndexBufferSubsystem::RemoveIndexBuffer not implemented.");
}

const VulkanBuffer* IndexBufferSystem::GetGlobalIndexBuffer() const
{
    return myBuffer;
}

void IndexBufferSystem::GrowBuffer(const uint inRequiredSize)
{
    ZoneScoped;
    if(!myBuffer)
    {
        vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
            .setSize(MathUtils::UpperPowerOfTwo(inRequiredSize))
            .setUsage(vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc);
        
        myBuffer = VulkanAllocator::AllocateBuffer_TS("GlobalIndexBuffer", createInfo, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE);
        return;
    }
    
    if(inRequiredSize <= myBuffer->GetSize())
        return;

    const vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
            .setSize(MathUtils::UpperPowerOfTwo(inRequiredSize))
            .setUsage(vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc);
    VulkanBuffer* newBuffer = VulkanAllocator::AllocateBuffer_TS("GlobalIndexBuffer", createInfo, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE);

    VulkanBuffer* oldBuffer = myBuffer;
    Engine::GetEngineSystem<RenderSystem>().AddUploadCommand_TS(this, [oldBuffer, newBuffer](vk::CommandBuffer inCommandBuffer)
    {
        const vk::BufferCopy copy = vk::BufferCopy().setSize(oldBuffer->GetSize()).setSrcOffset(0).setDstOffset(0);
        inCommandBuffer.copyBuffer(oldBuffer->GetAPIResource(), newBuffer->GetAPIResource(), {copy});
    });
    VulkanAllocator::DestroyBuffer_TS(oldBuffer);
    myBuffer = newBuffer;
}
