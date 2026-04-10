#pragma once
#include "Engine/Rendering/RenderingPasses/GraphicsPass.h"

class PrePass : public GraphicsPass
{
public:
    PrePass();
    void SetupAttachments() override;
    
    void DrawCall(vk::CommandBuffer inCommandBuffer) override;
};
