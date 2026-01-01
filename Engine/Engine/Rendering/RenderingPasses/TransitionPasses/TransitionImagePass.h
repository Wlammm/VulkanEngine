#pragma once
#include "Engine/Rendering/RenderingPasses/IRenderPass.h"

class VulkanImage;

class TransitionImagePass : public IRenderPass
{
public:
    TransitionImagePass(
        VulkanImage* inImage, 
        vk::AccessFlags inSrcAccess, 
        vk::AccessFlags inDstAccess, 
        vk::ImageLayout inSrcLayout, 
        vk::ImageLayout inDstLayout, 
        vk::PipelineStageFlags inSrcStage, 
        vk::PipelineStageFlags inDstStage,
        vk::ImageAspectFlags inAspectFlags);
    
    TransitionImagePass(
        vk::Image inImage, 
        vk::AccessFlags inSrcAccess, 
        vk::AccessFlags inDstAccess, 
        vk::ImageLayout inSrcLayout, 
        vk::ImageLayout inDstLayout, 
        vk::PipelineStageFlags inSrcStage, 
        vk::PipelineStageFlags inDstStage,
        vk::ImageAspectFlags inAspectFlags);
    
    void CreateResources() override;
    void DestroyResources() override;
    
    void Execute(vk::CommandBuffer inCommandBuffer) override;
    
private:
    vk::Image myImage = nullptr;
    vk::AccessFlags mySrcAccess;
    vk::AccessFlags myDstAccess;
    vk::ImageLayout mySrcLayout;
    vk::ImageLayout myDstLayout;
    vk::PipelineStageFlags mySrcStage;
    vk::PipelineStageFlags myDstStage;
    vk::ImageAspectFlags myAspectFlags;
};
