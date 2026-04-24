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

ResizableBuffer::~ResizableBuffer()
{
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
    myBuffer = nullptr;
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
}

void ResizableBuffer::SetData(const void* inData, const uint inSize, uint inOffset)
{
    CheckCapacity(inSize + inOffset);
    myBuffer->SetData(inData, inSize, inOffset);
}

void ResizableBuffer::MoveData(const uint inSourceOffset, const uint inDstOffset, const uint inSize)
{
    CheckCapacity(inSize + std::max(inDstOffset, inSourceOffset));

    // vkCmdCopyBuffer requires non-overlapping regions when src == dst, so bounce through a temp buffer.
    vk::BufferCreateInfo tmpCreateInfo = vk::BufferCreateInfo()
        .setSize(inSize)
        .setUsage(vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst);
    VulkanBuffer* tmpBuffer = VulkanAllocator::AllocateBuffer_TS(myBuffer->GetName() + "_MoveTemp", tmpCreateInfo, myBuffer->GetVmaMemoryUsage());

    VulkanCommandBuffer* readCmd = RenderSystem::CreateUploadCommandBuffer_TS();
    readCmd->GetAPIResource().copyBuffer(myBuffer->GetAPIResource(), tmpBuffer->GetAPIResource(),
        vk::BufferCopy().setSize(inSize).setSrcOffset(inSourceOffset).setDstOffset(0));
    List<ResourceUsage> readUsages{};
    readUsages.Emplace().SetToBuffer(myBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferRead);
    readUsages.Emplace().SetToBuffer(tmpBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferWrite);
    RenderSystem::QueueCommandBufferForUpload_TS(readCmd, readUsages);

    VulkanCommandBuffer* writeCmd = RenderSystem::CreateUploadCommandBuffer_TS();
    writeCmd->GetAPIResource().copyBuffer(tmpBuffer->GetAPIResource(), myBuffer->GetAPIResource(),
        vk::BufferCopy().setSize(inSize).setSrcOffset(0).setDstOffset(inDstOffset));
    List<ResourceUsage> writeUsages{};
    writeUsages.Emplace().SetToBuffer(tmpBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferRead);
    writeUsages.Emplace().SetToBuffer(myBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferWrite);
    RenderSystem::QueueCommandBufferForUpload_TS(writeCmd, writeUsages);

    VulkanAllocator::DestroyBuffer_TS(tmpBuffer);
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

    VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
    vk::BufferCopy copy = vk::BufferCopy().setSize(oldBuffer->GetSize());
    commandBuffer->GetAPIResource().copyBuffer(oldBuffer->GetAPIResource(), myBuffer->GetAPIResource(), copy);

    List<ResourceUsage> resourceUsages{};
    resourceUsages.Emplace().SetToBuffer(myBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferWrite);
    resourceUsages.Emplace().SetToBuffer(oldBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferRead);
    
	RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer, resourceUsages);

    VulkanAllocator::DestroyBuffer_TS(oldBuffer);
    OnBufferResized();
}

void ResizableBuffer::CheckCapacity(const uint inRequiredSize)
{
    if(inRequiredSize <= myBuffer->GetSize())
        return;

    Resize(inRequiredSize);
}