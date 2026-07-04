#include "EnginePch.h"
#include "StagingSystem.h"

#include "Engine/Engine.h"
#include "Engine/Utils/MathUtils.hpp"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"

// 4 mb default.
constexpr uint DefaultStagingBufferSize = 4'194'304;

static VulkanBuffer* AllocateStagingBuffer(const uint inSize)
{
    auto info = VulkanBuffer::StagingCreateInfo(inSize).setUsage(vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst);
    return VulkanAllocator::AllocateBuffer_TS("StagingSystem-StagingBuffer", info, VMA_MEMORY_USAGE_CPU_TO_GPU, true);
}

StagingSystem::StagingSystem()
{
    for(BufferData& data : myStagingBuffers)
    {
        data.myBuffer = AllocateStagingBuffer(DefaultStagingBufferSize);
        data.myOffset = 0;
    }

    PrepareThisFramesStagingBuffer();
}

StagingSystem::~StagingSystem()
{
    std::scoped_lock lock(myMutex);
    for(BufferData& data : myStagingBuffers)
    {
        VulkanAllocator::DestroyBuffer_TS(data.myBuffer);
        data.myBuffer = nullptr;
    }
}

StagingBuffer StagingSystem::GetStagingBufferWithSize_TS(const uint inSize)
{
    std::scoped_lock lock(myMutex);
    if(myCurrentStageData->myOffset + inSize > myCurrentStageData->myBuffer->GetSize())
    {
        LOG("Staging buffer overflow. Allocating a bigger staging buffer.");

        // Grow by swapping in a fresh buffer WITHOUT copying the old contents. Every already-queued
        // upload references the old VulkanBuffer directly (captured when it was staged), so nothing
        // ever reads old data through the new buffer — and destruction is frame-delayed, so those
        // pending copies stay valid. The previous ResizableBuffer::Resize path queued a GPU old->new
        // copy that executed AFTER the caller's CPU memcpy into the new buffer, overwriting the head
        // of the very allocation that triggered the grow with stale bytes.
        const uint newSize = MathUtils::UpperPowerOfTwo(myCurrentStageData->myOffset + inSize + 1);
        VulkanAllocator::DestroyBuffer_TS(myCurrentStageData->myBuffer);
        myCurrentStageData->myBuffer = AllocateStagingBuffer(newSize);
        myCurrentStageData->myOffset = 0;
    }

    StagingBuffer buffer = StagingBuffer(myCurrentStageData->myBuffer, inSize, myCurrentStageData->myOffset);

    myCurrentStageData->myOffset += inSize;
    return buffer;
}

void StagingSystem::Tick()
{
    PrepareThisFramesStagingBuffer();
}

void StagingSystem::PrepareThisFramesStagingBuffer()
{
    std::scoped_lock lock(myMutex);

    // The slot index must come from the monotonic engine frame counter, not the swapchain's
    // frame-in-flight index: there are NumStagingSlots (FrameLag + 1) slots, see the header comment.
    const uint frameIndex = Engine::GetFrameIndex();
    myCurrentStageData = &myStagingBuffers[frameIndex % NumStagingSlots];

    // We do this as first frame we will get 2 ticks here before we get a new frame index. We cant overwrite this data then or shit gets corrupted.
    if(frameIndex != myLastFrameIndex)
        myCurrentStageData->myOffset = 0;

    myLastFrameIndex = frameIndex;
}
