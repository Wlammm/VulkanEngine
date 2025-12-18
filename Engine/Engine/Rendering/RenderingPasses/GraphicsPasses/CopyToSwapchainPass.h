#pragma once
#include "Engine/Rendering/RenderingPasses/GraphicsPass.h"

class CopyToSwapchainPass : public GraphicsPass
{
public:
    CopyToSwapchainPass(VulkanImage* inSourceImage);
    
    void SetupDescriptors() override;
    void SetupAttachments() override;
    
    vk::SampleCountFlagBits GetNumSamples() const override;
    
    List<vk::RenderingAttachmentInfo> GetDynamicColorAttachments() override;
    vk::RenderingAttachmentInfo GetDynamicDepthAttachments() override;
    
    void DrawCall(vk::CommandBuffer inCommandBuffer) override;
    
private:
    VulkanImage* mySourceTexture = nullptr;
};
