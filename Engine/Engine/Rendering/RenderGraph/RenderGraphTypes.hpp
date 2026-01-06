#pragma once

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
    
    ResourceState myRequiredState;
    
    bool myIsReadOnly = true;
};