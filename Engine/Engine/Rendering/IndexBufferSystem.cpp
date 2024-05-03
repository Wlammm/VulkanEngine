#include "EnginePch.h"
#include "IndexBufferSystem.h"

#include "ECS/Systems/RenderSystem.h"
#include "Utils/MathUtils.hpp"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"

IndexBufferSystem::IndexBufferSystem()
{
    
}

IndexBufferSystem::~IndexBufferSystem()
{
    myUsedBufferSize = 0;
    myCurrentIndexOffset = 0;
    myNextHandleID = 0;
    myIndexBuffers.clear();
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
}

IndexBufferHandle IndexBufferSystem::UploadIndexData(const List<uint>& inIndices)
{
    check(!inIndices.IsEmpty() && "Vertices should not be empty.");

    const uint sizeIncrease = inIndices.size() * sizeof(uint);
    const uint requiredSize = myUsedBufferSize + sizeIncrease;
    GrowBuffer(requiredSize);

    myBuffer->SetData(inIndices.data(), sizeIncrease, myUsedBufferSize);
    
    IndexBufferData data{};
    data.myOffset = myCurrentIndexOffset;
    data.myIndexCount = inIndices.size();
    data.myID = ++myNextHandleID;
    myIndexBuffers.insert({data.myID, data});
    
    myUsedBufferSize += sizeIncrease;
    myCurrentIndexOffset += static_cast<uint>(inIndices.size());
    
    return data.myID;
}

void IndexBufferSystem::RemoveIndexBuffer(const IndexBufferHandle inHandle)
{
    LOG_WARNING("IndexBufferSystem::RemoveIndexBuffer not implemented.");
}

const IndexBufferData& IndexBufferSystem::GetIndexBufferData(const IndexBufferHandle inHandle) const
{
    check(myIndexBuffers.contains(inHandle) && "Invalid handle.");
    return myIndexBuffers.at(inHandle);
}

const VulkanBuffer* IndexBufferSystem::GetGlobalIndexBuffer() const
{
    return myBuffer;
}

void IndexBufferSystem::GrowBuffer(const uint inRequiredSize)
{
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
    RenderSystem::AddUploadCommand_TS(this, [oldBuffer, newBuffer](vk::CommandBuffer inCommandBuffer)
    {
        const vk::BufferCopy copy = vk::BufferCopy().setSize(oldBuffer->GetSize()).setSrcOffset(0).setDstOffset(0);
        inCommandBuffer.copyBuffer(oldBuffer->GetAPIResource(), newBuffer->GetAPIResource(), {copy});
    });
    VulkanAllocator::DestroyBuffer_TS(oldBuffer);
    myBuffer = newBuffer;
}
