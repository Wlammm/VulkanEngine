#include "EnginePch.h"
#include "GPUResourceManager.h"

#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"

GPUResourceManager::GPUResourceManager()
{
    check(!myInstance);
    myInstance = this;
}

GPUResourceManager::~GPUResourceManager()
{
    myInstance = nullptr;
    
    for (BufferResource& buffer : myBuffers)
    {
        // TODO: Fix this hack later on.
        if (VulkanBuffer* vulkanBuffer = dynamic_cast<VulkanBuffer*>(buffer.myBuffer))
        {
            VulkanAllocator::DestroyBuffer_TS(vulkanBuffer);
        }
        else
        {
            del(buffer.myBuffer);
        }
    }
    
    myBuffers.Clear();
}
