#include "EnginePch.h"
#include "ResizableBuffer.h"

#include <stacktrace>

#include "Engine/Engine.h"
#include "VulkanAllocator.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "Engine/Rendering/RenderSystem.h"
#include "Engine/Utils/MathUtils.hpp"

ResizableBuffer::ResizableBuffer(VulkanBuffer* inBuffer)
{
    myBuffer = inBuffer;
}

VulkanBuffer* ResizableBuffer::GetBuffer() const
{
    return myBuffer;
}

MulticastDelegate<void()>* ResizableBuffer::GetOnBufferResized() const
{
    return &OnBufferResized;
}

void ResizableBuffer::CopyDataFromBuffer(VulkanBuffer* inStagingBuffer, const uint inSize, uint inOffset)
{
    CheckCapacity(inSize + inOffset);
    myBuffer->CopyDataFromBuffer(inStagingBuffer, inSize, inOffset);

    VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
    
    vk::BufferMemoryBarrier barrier = vk::BufferMemoryBarrier()
        .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
        .setDstAccessMask(vk::AccessFlagBits::eTransferWrite)
        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setBuffer(myBuffer->GetAPIResource())
        .setOffset(0)
        .setSize(VK_WHOLE_SIZE);

    commandBuffer->GetAPIResource().pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eTransfer,
                vk::DependencyFlags{},
                nullptr,
                barrier,
                nullptr);
	RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer);
}

void ResizableBuffer::SetData(const void* inData, const uint inSize, uint inOffset)
{
    CheckCapacity(inSize + inOffset);
    myBuffer->SetData(inData, inSize, inOffset);

    // If the buffer isnt writable we need to insert a barrier for other copies to wait for this to finish or we cant guarantee that the memory is correct.
    if(!myBuffer->IsMappable())
    {
        VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
        
        vk::BufferMemoryBarrier barrier = vk::BufferMemoryBarrier()
        .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
        .setDstAccessMask(vk::AccessFlagBits::eTransferWrite)
        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setBuffer(myBuffer->GetAPIResource())
        .setOffset(0)
        .setSize(VK_WHOLE_SIZE);

        commandBuffer->GetAPIResource().pipelineBarrier(
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::DependencyFlags{},
                    nullptr,
                    barrier,
                    nullptr);
	    RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer);
    }
}

void ResizableBuffer::MoveData(const uint inSourceOffset, const uint inDstOffset, const uint inSize)
{
    CheckCapacity(inSize + std::max(inDstOffset, inSourceOffset));
    
    VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
    vk::BufferCopy copy = vk::BufferCopy().setSize(inSize).setSrcOffset(inSourceOffset).setDstOffset(inDstOffset);
    commandBuffer->GetAPIResource().copyBuffer(myBuffer->GetAPIResource(), myBuffer->GetAPIResource(), copy);
    
    vk::BufferMemoryBarrier bufferMemoryBarrier{};
    bufferMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
    bufferMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eMemoryRead;
    bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferMemoryBarrier.buffer = myBuffer->GetAPIResource();
    bufferMemoryBarrier.offset = inDstOffset;
    bufferMemoryBarrier.size = inSize;
    
    vk::PipelineStageFlags srcStageMask = vk::PipelineStageFlagBits::eTransfer;
    vk::PipelineStageFlags dstStageMask = vk::PipelineStageFlagBits::eComputeShader;

    commandBuffer->GetAPIResource().pipelineBarrier(
        srcStageMask,              
        dstStageMask,              
        {},                        
        nullptr,                   
        bufferMemoryBarrier,       
        nullptr                    
    );
	RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer);
}

void ResizableBuffer::Resize(const uint inRequiredSize)
{
    if(inRequiredSize <= myBuffer->GetSize())
        return;

    LOG("Resizing buffer: %s - FrameIndex: %i", myBuffer->GetName().c_str(), Engine::GetFrameIndex());
    uint newSize = MathUtils::UpperPowerOfTwo(inRequiredSize);

    VulkanBuffer* oldBuffer = myBuffer;
    vk::BufferCreateInfo createInfo = myBuffer->GetCreateInfo();
    createInfo.setSize(newSize);
    myBuffer = VulkanAllocator::AllocateBuffer_TS(myBuffer->GetName(), createInfo, myBuffer->GetVmaMemoryUsage(), myBuffer->IsMappable());
    myHasActiveUpload = true;
    
    VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
    myHasActiveUpload = false;
    vk::BufferCopy copy = vk::BufferCopy().setSize(oldBuffer->GetSize());
    commandBuffer->GetAPIResource().copyBuffer(oldBuffer->GetAPIResource(), myBuffer->GetAPIResource(), copy);

    // Ensure the copy is finished before any further copies to this buffer
    vk::BufferMemoryBarrier barrier = vk::BufferMemoryBarrier()
        .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
        .setDstAccessMask(vk::AccessFlagBits::eTransferWrite)
        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setBuffer(myBuffer->GetAPIResource())
        .setOffset(0)
        .setSize(VK_WHOLE_SIZE);

    commandBuffer->GetAPIResource().pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eTransfer,
                vk::DependencyFlags{},
                nullptr,
                barrier,
                nullptr);
	RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer);

    VulkanAllocator::DestroyBuffer_TS(oldBuffer);
    OnBufferResized();
}

void ResizableBuffer::CheckCapacity(const uint inRequiredSize)
{
    if(inRequiredSize <= myBuffer->GetSize())
        return;

    Resize(inRequiredSize);
}