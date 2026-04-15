#pragma once

class IGPUAccelerationStructure;
class IGPUBuffer;
class VulkanImage;

struct ResourceState
{
    vk::PipelineStageFlags myStage;
    vk::AccessFlags myAccessFlags;
    vk::ImageLayout myLayout;
    uint myQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
};

struct ResourceUsage
{
    VulkanImage* myImage = nullptr;
    IGPUBuffer* myBuffer = nullptr;
    IGPUAccelerationStructure* myAccelerationStructure = nullptr;

    ResourceState myRequiredState;

    bool myIsReadOnly = true;

    void SetToImage(VulkanImage* inImage, vk::PipelineStageFlags inStageFlags, vk::AccessFlags inAccess, vk::ImageLayout inLayout)
    {
        myImage = inImage;
        myRequiredState = { inStageFlags, inAccess, inLayout };
        myIsReadOnly = (inAccess & (vk::AccessFlagBits::eMemoryWrite | vk::AccessFlagBits::eShaderWrite | vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite)) == vk::AccessFlagBits::eNone;
    }

    void SetToBuffer(IGPUBuffer* inBuffer, vk::PipelineStageFlags inStages, vk::AccessFlags inAccessFlags)
    {
        myBuffer = inBuffer;
        myRequiredState = { inStages, inAccessFlags, vk::ImageLayout::eUndefined };
        myIsReadOnly = (inAccessFlags & (vk::AccessFlagBits::eMemoryWrite | vk::AccessFlagBits::eShaderWrite | vk::AccessFlagBits::eTransferWrite)) == vk::AccessFlagBits::eNone;
    }

    void SetToAccelerationStructure(IGPUAccelerationStructure* inAS, vk::PipelineStageFlags inStages, vk::AccessFlags inAccessFlags)
    {
        myAccelerationStructure = inAS;
        myRequiredState = { inStages, inAccessFlags, vk::ImageLayout::eUndefined };
        myIsReadOnly = (inAccessFlags & vk::AccessFlagBits::eAccelerationStructureWriteKHR) == vk::AccessFlagBits::eNone;
    }
};