#pragma once
#include "Engine/Rendering/RenderingPasses/GraphicsPass.h"

class CopyTexturePass : public GraphicsPass
{
public:
    CopyTexturePass(VulkanImage* inSourceTexture, VulkanImage* inTargetTexture);

    void SetupAttachments() override;
    void SetupDescriptors() override;
    
    void DrawCall(vk::CommandBuffer inCommandBuffer) override;
    
    
private:
    VulkanImage* mySourceTexture = nullptr;
    VulkanImage* myTargetTexture = nullptr;
};
