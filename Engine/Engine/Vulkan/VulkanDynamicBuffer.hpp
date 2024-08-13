#pragma once
#include "Engine.h"
#include "VulkanAllocator.h"
#include "VulkanBuffer.h"
#include "Delegates/MulticastDelegate.hpp"
#include "Rendering/RenderSystem.h"
#include "Utils/MathUtils.hpp"

/*
 * This is a GPU dynamic buffer. It grows automatically when you add an item. Keeps track of free indices and reuses them before allocating new ones at the back.
 * This means the buffer is not defragmented so there is no way to iterate through it. Keep track of your indices yourself.
*/
class IVulkanDynamicBuffer
{
public:
    virtual VulkanBuffer* GetBuffer() const = 0;
    virtual uint GetNum() const = 0;

    mutable MulticastDelegate<void()> OnBufferRecreated;
};

template<typename ElementType>
class VulkanDynamicBuffer : public IVulkanDynamicBuffer
{
public:
    VulkanDynamicBuffer() = delete;
    VulkanDynamicBuffer(const VulkanDynamicBuffer& inOther) = delete;
    VulkanDynamicBuffer& operator=(const VulkanDynamicBuffer& inOther) = delete;
    
    VulkanDynamicBuffer(const std::string& inName, const vk::BufferCreateInfo& inCreateInfo)
    {
        myName = inName;
        myCreateInfo = inCreateInfo;

        // These are always required for dynamic buffers.
        myCreateInfo.usage |= vk::BufferUsageFlagBits::eTransferSrc;
        myCreateInfo.usage |= vk::BufferUsageFlagBits::eTransferDst;

        CheckCapacityForAdd(sizeof(ElementType));
    }

    ~VulkanDynamicBuffer()
    {
        if(myBuffer)
            VulkanAllocator::DestroyBuffer_TS(myBuffer);
    }

    uint Add(const ElementType& inValue)
    {
        // Create a buffer if we dont have one already.
        if(!myBuffer)
            CheckCapacityForAdd(sizeof(ElementType));

        // If there are no free indices in the middle, we need to add the new item at the end.
        if(myFreeIndices.empty())
        {
            CheckCapacityForAdd(myUsedSizeInBytes + sizeof(ElementType));
            myBuffer->SetData(&inValue, sizeof(ElementType), myUsedSizeInBytes);
            myUsedSizeInBytes += sizeof(ElementType);
            myLastIndex++;
            return myLastIndex - 1;
        }

        uint index = myFreeIndices.front();
        myFreeIndices.pop();
        myBuffer->SetData(&inValue, sizeof(ElementType), sizeof(ElementType) * index);
        return index;
    }

    // We dont remove. ty
    //void RemoveIndex(const uint inIndex)
    //{
    //    myFreeIndices.push(inIndex);
    //}

    virtual VulkanBuffer* GetBuffer() const override final
    {
        return myBuffer;
    }

    uint GetNum() const override final
    {
        return myLastIndex;
    }

private:
    // Makes sure the buffer has enough storage required. inRequiredSize = total buffer size in bytes.
    void CheckCapacityForAdd(const uint inRequiredSize)
    {
        if(!myBuffer)
        {
            vk::BufferCreateInfo createInfo = myCreateInfo;
            createInfo.setSize(MathUtils::UpperPowerOfTwo(inRequiredSize));
        
            myBuffer = VulkanAllocator::AllocateBuffer_TS(myName, createInfo, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE);
            OnBufferRecreated();
            return;
        }
    
        if(inRequiredSize <= myBuffer->GetSize())
            return;

        vk::BufferCreateInfo createInfo = myCreateInfo;
        createInfo.setSize(MathUtils::UpperPowerOfTwo(inRequiredSize));
        VulkanBuffer* newBuffer = VulkanAllocator::AllocateBuffer_TS(myName, createInfo, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE);

        VulkanBuffer* oldBuffer = myBuffer;
        Engine::GetEngineSystem<RenderSystem>().AddUploadCommand_TS(this, [oldBuffer, newBuffer](vk::CommandBuffer inCommandBuffer)
        {
            const vk::BufferCopy copy = vk::BufferCopy().setSize(oldBuffer->GetSize()).setSrcOffset(0).setDstOffset(0);
            inCommandBuffer.copyBuffer(oldBuffer->GetAPIResource(), newBuffer->GetAPIResource(), {copy});
        });
        VulkanAllocator::DestroyBuffer_TS(oldBuffer);
        myBuffer = newBuffer;
        OnBufferRecreated();
    }
    
private:
    VulkanBuffer* myBuffer = nullptr;
    uint myUsedSizeInBytes = 0;
    uint myLastIndex = 0;
    
    std::string myName = "";
    vk::BufferCreateInfo myCreateInfo;

    std::queue<uint> myFreeIndices{};
};
