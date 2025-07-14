#pragma once

class VulkanCommandBuffer
{
public:
    VulkanCommandBuffer(vk::CommandBuffer inBuffer, vk::CommandPool inPool);
    ~VulkanCommandBuffer();

    vk::CommandBuffer GetAPIResource() const;

private:
    vk::CommandBuffer myCommandBuffer;
    vk::CommandPool myCommandPool;
};
