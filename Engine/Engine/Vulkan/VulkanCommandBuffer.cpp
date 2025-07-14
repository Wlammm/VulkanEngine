#include "EnginePch.h"
#include "VulkanCommandBuffer.h"

#include "VulkanContext.h"
#include "VulkanDevice.h"

VulkanCommandBuffer::VulkanCommandBuffer(vk::CommandBuffer inBuffer, vk::CommandPool inPool)
{
    myCommandBuffer = inBuffer;
    myCommandPool = inPool;
}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
    VulkanContext::GetDevice()->freeCommandBuffers(myCommandPool, myCommandBuffer);
    VulkanContext::GetDevice()->destroyCommandPool(myCommandPool);
}

vk::CommandBuffer VulkanCommandBuffer::GetAPIResource() const
{
    return myCommandBuffer;
}
