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

void IndexBufferSystem::Tick()
{
    UploadAllQueuedIndexBuffers();    
}

void IndexBufferSystem::UploadIndexData(const List<uint>& inIndices, IndexBuffer* inBuffer)
{
    const uint sizeIncrease = inIndices.size() * sizeof(uint);
    const uint requiredSize = myUsedBufferSize + sizeIncrease;
    GrowBuffer(requiredSize);

    myBuffer->SetData(inIndices.data(), sizeIncrease, myUsedBufferSize);
    
    inBuffer->myOffset = myCurrentIndexOffset;
    inBuffer->myIndexCount = inIndices.size();
    myIndexBuffers.Add(inBuffer);
    
    myUsedBufferSize += sizeIncrease;
    myCurrentIndexOffset += static_cast<uint>(inIndices.size());
}

void IndexBufferSystem::RemoveIndexBuffer(const IndexBuffer* inBuffer)
{
    LOG_WARNING("IndexBufferSubsystem::RemoveIndexBuffer not implemented.");
}

IndexBuffer* IndexBufferSystem::UploadIndexBuffer_TS(const List<uint>& inIndices)
{
    check(!inIndices.IsEmpty() && "Indices should not be empty.");
    IndexBuffer* buffer = new IndexBuffer();
    
    std::shared_ptr<IndexUploadData> uploadData = std::make_shared<IndexUploadData>();
    uploadData->myIndices = inIndices;
    uploadData->myBuffer = buffer;
    myQueuedUploadData.Add(uploadData);
    return buffer;
}

const VulkanBuffer* IndexBufferSystem::GetGlobalIndexBuffer() const
{
    return myBuffer;
}

void IndexBufferSystem::UploadAllQueuedIndexBuffers()
{
    ZoneScoped;
    myQueuedUploadData.Lock();
    for(std::shared_ptr<IndexUploadData> uploadData : myQueuedUploadData)
    {
        UploadIndexData(uploadData->myIndices, uploadData->myBuffer);
    }
    myQueuedUploadData.Clear();
    myQueuedUploadData.Unlock();
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
