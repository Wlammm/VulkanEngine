#pragma once
#include <algorithm>

#include "IGPUBuffer.hpp"
#include "Engine/Engine.h"
#include "Engine/Delegates/MulticastDelegate.hpp"
#include "Engine/Rendering/RenderSystem.h"
#include "Engine/Utils/MathUtils.hpp"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanCommandBuffer.h"

#define ENABLE_GPU_DEBUGGING 1



/*
 * A gpu version of the CPU List container.
 */
template<typename ElementType> 
class GPUList : public IGPUBuffer
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
    MulticastDelegate<void()>* GetOnBufferResized() const override { return &OnGPUBufferResized; };
    
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
        
        List<ResourceUsage> resourceUsages{};
        resourceUsages.Emplace().SetToBuffer(this, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferWrite);
        
        RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer, resourceUsages);
        
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
        return sizeof(ElementType) * inIndex;
    }
    
    void SetSize(const uint inSize)
    {
        mySize = inSize;
    }
    
    void EnsureCapacityForAdds(const uint inNumAdds)
    {
        uint requiredCapacity = mySize + inNumAdds;
        
#if ENABLE_GPU_DEBUGGING
        myCpuDebuggingList.Resize(requiredCapacity);
#endif
        
        if (myCapacity >= requiredCapacity)
            return;
        
        SetCapacity(MathUtils::UpperPowerOfTwo(requiredCapacity));
    }
    
    // Sets the capacity to this. 
    void SetCapacity(const uint inNewCapacity)
    {
        myCapacity = inNewCapacity;
        mySize = std::min(mySize, inNewCapacity);
        
#if ENABLE_GPU_DEBUGGING
        myCpuDebuggingList.Resize(inNewCapacity);
#endif
        
        VulkanBuffer* oldBuffer = myBuffer;
        
        // Create new buffer with required size.
        vk::BufferCreateInfo createInfo = myCreateInfo;
        createInfo.setSize(inNewCapacity * sizeof(ElementType));
        myBuffer = VulkanAllocator::AllocateBuffer_TS(myBufferName, createInfo, myBufferMemoryUsage, false);

        if (oldBuffer == nullptr)
        {
            SetSize(mySize);
            OnGPUBufferResized.Invoke();
            return;
        }
        
        LOG("Resizing GPU List: %s - FrameIndex: %i", myBuffer->GetName().c_str(), Engine::GetFrameIndex());
        
        // Copy old buffer into new buffer.
        VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
        vk::BufferCopy copy = vk::BufferCopy().setSize(mySize * sizeof(ElementType));
        commandBuffer->GetAPIResource().copyBuffer(oldBuffer->GetAPIResource(), myBuffer->GetAPIResource(), copy);
        
        List<ResourceUsage> resourceUsages{};
        resourceUsages.Emplace().SetToBuffer(this, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferWrite);
        resourceUsages.Emplace().SetToBuffer(oldBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferRead);
        
        // Queue uploads to render system and destroy old buffer. 
        RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer, resourceUsages);
        VulkanAllocator::DestroyBuffer_TS(oldBuffer);
        OnGPUBufferResized.Invoke();
    }
    
    uint Size() const
    {
        return mySize;
    }
    
    // Grows the list by a specific amount without initializing the data, 
    // returning the start index of the new range.
    uint Grow(const uint inAmount)
    {
        EnsureCapacityForAdds(inAmount);
        uint startOffset = mySize;
        SetSize(mySize + inAmount);
        return startOffset;
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
