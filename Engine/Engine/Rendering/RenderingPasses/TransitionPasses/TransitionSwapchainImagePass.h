#pragma once
#include "Engine/Rendering/RenderingPasses/IRenderPass.h"

class TransitionSwapchainImagePass : public IRenderPass
{
public:
    TransitionSwapchainImagePass(
        vk::AccessFlagBits inSrcAccess, 
        vk::AccessFlagBits inDstAccess, 
        vk::ImageLayout inSrcLayout, 
        vk::ImageLayout inDstLayout, 
        vk::PipelineStageFlagBits inSrcStage, 
        vk::PipelineStageFlagBits inDstStage);
    
    void CreateResources() override;
    void DestroyResources() override;
    
    void Execute(vk::CommandBuffer inCommandBuffer) override;
    
private:
    vk::AccessFlagBits mySrcAccess;
    vk::AccessFlagBits myDstAccess;
    vk::ImageLayout mySrcLayout;
    vk::ImageLayout myDstLayout;
    vk::PipelineStageFlagBits mySrcStage;
    vk::PipelineStageFlagBits myDstStage;
};
