#pragma once
#include "Engine/Rendering/RenderingPasses/IRenderPass.h"

class VulkanImage;

class TransitionImagePass : public IRenderPass
{
public:
    TransitionImagePass(
        VulkanImage* inImage, 
        vk::AccessFlagBits inSrcAccess, 
        vk::AccessFlagBits inDstAccess, 
        vk::ImageLayout inSrcLayout, 
        vk::ImageLayout inDstLayout, 
        vk::PipelineStageFlagBits inSrcStage, 
        vk::PipelineStageFlagBits inDstStage,
        vk::ImageAspectFlags inAspectFlags);
    TransitionImagePass(
        vk::Image inImage, 
        vk::AccessFlagBits inSrcAccess, 
        vk::AccessFlagBits inDstAccess, 
        vk::ImageLayout inSrcLayout, 
        vk::ImageLayout inDstLayout, 
        vk::PipelineStageFlagBits inSrcStage, 
        vk::PipelineStageFlagBits inDstStage,
        vk::ImageAspectFlags inAspectFlags);
    
    void CreateResources() override;
    void DestroyResources() override;
    
    void Execute(vk::CommandBuffer inCommandBuffer) override;
    
private:
    vk::Image myImage = nullptr;
    vk::AccessFlagBits mySrcAccess;
    vk::AccessFlagBits myDstAccess;
    vk::ImageLayout mySrcLayout;
    vk::ImageLayout myDstLayout;
    vk::PipelineStageFlagBits mySrcStage;
    vk::PipelineStageFlagBits myDstStage;
    vk::ImageAspectFlags myAspectFlags;
};
