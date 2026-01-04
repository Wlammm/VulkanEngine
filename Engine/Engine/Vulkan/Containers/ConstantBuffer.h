#pragma once
#include "IGPUBuffer.hpp"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"

template <typename T>
class ConstantBuffer : public IGPUBuffer
{
public:
    ConstantBuffer(const std::string& inBufferName)
    {
        // TODO: Should we allow these to not be mappable?
        myBuffer = VulkanAllocator::AllocateBuffer_TS(
            inBufferName, 
            VulkanBuffer::UniformBufferCreateInfo(sizeof(T)),
            VMA_MEMORY_USAGE_AUTO,
            true);
    }

    ~ConstantBuffer()
    {
        VulkanAllocator::DestroyBuffer_TS(myBuffer);
    }
    
    void SetData(const T& inData)
    {
        myBuffer->SetData(inData);
    }
    
    VulkanBuffer* GetBuffer() const override
    {
        return myBuffer;
    }
    
    MulticastDelegate<void()>* GetOnBufferResized() const override
    {
        return nullptr;
    }

private:
    VulkanBuffer* myBuffer = nullptr;
};
