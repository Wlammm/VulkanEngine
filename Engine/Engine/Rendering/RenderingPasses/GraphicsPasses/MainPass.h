#pragma once
#include "Engine/Rendering/RenderingPasses/GraphicsPass.h"

class MainPass : public GraphicsPass
{
public:
    MainPass();
    
    void SetupDescriptors() override;
    void DrawCall(vk::CommandBuffer inCommandBuffer) override;
};
