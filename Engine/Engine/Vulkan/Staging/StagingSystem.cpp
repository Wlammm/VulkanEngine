#include "EnginePch.h"
#include "StagingSystem.h"

#include "Utils/MathUtils.hpp"
#include "Vulkan/ResizableBuffer.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanSwapchain.h"

// 4 mb default.
constexpr uint DefaultStagingBufferSize = 4'194'304;

StagingSystem::StagingSystem()
{
    for(int i = 0; i < 3; ++i)
    {
        BufferData data;
        auto info = VulkanBuffer::StagingCreateInfo(DefaultStagingBufferSize).setUsage(vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst);
        data.myBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("StagingSystem-StagingBuffer", info, VMA_MEMORY_USAGE_CPU_TO_GPU, true ));
        myStagingBuffers[i] = data;
    }

    PrepareThisFramesStagingBuffer();
}

StagingSystem::~StagingSystem()
{
    for(int i = 0; i < 3; ++i)
    {
        VulkanAllocator::DestroyBuffer_TS(myStagingBuffers[i].myBuffer);
        myStagingBuffers[i].myBuffer = nullptr;
    }
}

StagingBuffer StagingSystem::GetStagingBufferWithSize(const uint inSize)
{
    if(myCurrentStageData->myOffset + inSize > myCurrentStageData->myBuffer->GetBuffer()->GetSize())
    {
        LOG("Staging buffer overflow. Resizing");
        myCurrentStageData->myBuffer->Resize(MathUtils::UpperPowerOfTwo(myCurrentStageData->myOffset + inSize + 1));
    }
    
    StagingBuffer buffer = StagingBuffer(myCurrentStageData->myBuffer->GetBuffer(), inSize, myCurrentStageData->myOffset);
    
    myCurrentStageData->myOffset += inSize;
    return buffer;
}

void StagingSystem::Tick()
{
    PrepareThisFramesStagingBuffer();
}

void StagingSystem::PrepareThisFramesStagingBuffer()
{
    myCurrentStageData = &myStagingBuffers[VulkanContext::GetSwapChain().GetFrameIndex()];

    // We do this as first frame we will get 2 ticks here before we get a new frame index. We cant overwrite this data then or shit gets corrupted.
    if(VulkanContext::GetSwapChain().GetFrameIndex() != myLastFrameIndex)
        myCurrentStageData->myOffset = 0;

    myLastFrameIndex = VulkanContext::GetSwapChain().GetFrameIndex();
}


