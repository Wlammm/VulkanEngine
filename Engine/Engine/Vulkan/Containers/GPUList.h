#pragma once
#include <algorithm>

#include "Engine/Engine.h"
#include "Engine/Delegates/MulticastDelegate.hpp"
#include "Engine/Rendering/RenderSystem.h"
#include "Engine/Utils/MathUtils.hpp"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanCommandBuffer.h"

#define ENABLE_GPU_DEBUGGING 1

class IGPUList
{
public:
    virtual ~IGPUList() = default;
    virtual VulkanBuffer* GetBuffer() const = 0;
    virtual MulticastDelegate<void()>& GetOnBufferResized() const = 0;
};

/*
 * A gpu version of the CPU List container.
 */
template<typename ElementType> 
class GPUList : public IGPUList
{
public:
    ~GPUList()
    {
        if (myBuffer)
        {
            VulkanAllocator::DestroyBuffer_TS(myBuffer);
            myBuffer = nullptr;
        }
    }
    
    mutable MulticastDelegate<void()> OnGPUBufferResized;
    
    VulkanBuffer* GetBuffer() const override { return myBuffer;}
    MulticastDelegate<void()>& GetOnBufferResized() const override { return OnGPUBufferResized; };
    
    GPUList(const vk::BufferCreateInfo& inCreateInfo, const std::string& inBufferName, const VmaMemoryUsage inMemoryUsage, const uint inCapacity = 4)
    {
        myCreateInfo = inCreateInfo;
        myBufferName = inBufferName;
        myBufferMemoryUsage = inMemoryUsage;
        SetCapacity(inCapacity);
    }
    GPUList(const GPUList&) = delete;
    void operator=(const GPUList&) = delete;
    
    void Add(const ElementType& inElement)
    {
        EnsureCapacityForAdds(1);
        SetDataAtIndex(inElement, mySize);
        
#if ENABLE_GPU_DEBUGGING
        myCpuDebuggingList[mySize] = inElement;
#endif
        
        SetSize(mySize + 1);
    }
    
    void SetDataAtIndex(const ElementType& inElement, const uint inIndex)
    {
        myBuffer->SetData(&inElement, sizeof(ElementType), GetOffsetToIndex(inIndex));
#if ENABLE_GPU_DEBUGGING
        myCpuDebuggingList[inIndex] = inElement;
#endif
    }

    // Moves the last element to the current index slot and decrement size. Last inserted element will now be on inIndex slot instead.
    void RemoveIndex(const uint inIndex)
    {
        if (inIndex == mySize - 1)
        {
            SetSize(mySize - 1);
            return;
        }
        
        VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
        vk::BufferCopy copy = vk::BufferCopy().setSize(sizeof(ElementType)).setSrcOffset(GetOffsetToIndex(mySize - 1)).setDstOffset(GetOffsetToIndex(inIndex));
        commandBuffer->GetAPIResource().copyBuffer(myBuffer->GetAPIResource(), myBuffer->GetAPIResource(), copy);
    
        vk::BufferMemoryBarrier bufferMemoryBarrier{};
        bufferMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        bufferMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eMemoryRead;
        bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        bufferMemoryBarrier.buffer = myBuffer->GetAPIResource();
        bufferMemoryBarrier.offset = GetOffsetToIndex(inIndex);
        bufferMemoryBarrier.size = sizeof(ElementType);
    
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
        
        SetSize(mySize - 1);
        
#if ENABLE_GPU_DEBUGGING
        myCpuDebuggingList[inIndex] = myCpuDebuggingList.Last();
        myCpuDebuggingList.RemoveLast();
#endif
    }
    
    // Removes the last element in the list.
    void RemoveLast()
    {
        SetSize(mySize - 1);
        
#if ENABLE_GPU_DEBUGGING
        myCpuDebuggingList.RemoveLast();  
#endif
    }
    
private:
    
    const uint GetOffsetToIndex(const uint inIndex)
    {
        constexpr int numElementsOffset = 16;
        
        return sizeof(ElementType) * inIndex + numElementsOffset;
    }
    
    void SetSize(const uint inSize)
    {
        mySize = inSize;
        myBuffer->SetData(&mySize, sizeof(uint), 0);
    }
    
    void EnsureCapacityForAdds(const uint inNumAdds)
    {
        uint requiredCapacity = mySize + inNumAdds;
        if (myCapacity >= requiredCapacity)
            return;
        
        SetCapacity(MathUtils::UpperPowerOfTwo(requiredCapacity));
    }
    
    // Sets the capacity to this. 
    void SetCapacity(const uint inNewCapacity)
    {
        myCapacity = inNewCapacity;
        mySize = std::min(mySize, inNewCapacity);
        
        VulkanBuffer* oldBuffer = myBuffer;
        
        // Create new buffer with required size.
        vk::BufferCreateInfo createInfo = myCreateInfo;
        createInfo.setSize(inNewCapacity * sizeof(ElementType) + 16);
        myBuffer = VulkanAllocator::AllocateBuffer_TS(myBufferName, createInfo, myBufferMemoryUsage, false);

#if ENABLE_GPU_DEBUGGING
        myCpuDebuggingList.Resize(inNewCapacity);
#endif
        if (oldBuffer == nullptr)
        {
            SetSize(mySize);
            OnGPUBufferResized.Invoke();
            return;
        }
        
        LOG("Resizing GPU List: %s - FrameIndex: %i", myBuffer->GetName().c_str(), Engine::GetFrameIndex());
        
        // Copy old buffer into new buffer.
        VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
        vk::BufferCopy copy = vk::BufferCopy().setSize(mySize * sizeof(ElementType) + 16);
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
        
        
        // Queue uploads to render system and destroy old buffer. 
        RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer);
        VulkanAllocator::DestroyBuffer_TS(oldBuffer);
        OnGPUBufferResized.Invoke();
    }
    
    
private:
#if ENABLE_GPU_DEBUGGING
    List<ElementType> myCpuDebuggingList;
#endif
    
    uint myCapacity = 0;
    uint mySize = 0;
    
    vk::BufferCreateInfo myCreateInfo = vk::BufferCreateInfo();
    std::string myBufferName = "";
    VmaMemoryUsage myBufferMemoryUsage;
    VulkanBuffer* myBuffer = nullptr;
};
