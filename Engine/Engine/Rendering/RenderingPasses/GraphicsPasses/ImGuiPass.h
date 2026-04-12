#pragma once
#include "Engine/Rendering/RenderingPasses/GraphicsPass.h"

class ImGuiPass : public IRenderPass
{
public:
    ImGuiPass() = default;
  
    void CreateResources() override;
    void DestroyResources() override;
    
    void PreExecute() override;
    void Execute(vk::CommandBuffer inCommandBuffer) override;
    
private:
    vk::RenderPass myRenderPass;
    List<vk::Framebuffer> myFrameBuffers;

    
};
