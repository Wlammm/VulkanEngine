#pragma once
#include "Engine/Rendering/RenderingPasses/GraphicsPass.h"

class NoDepthPass : public GraphicsPass
{
public:
    NoDepthPass();
    
    void SetupAttachments() override;
    
    void DrawCall(vk::CommandBuffer inCommandBuffer) override;
};
