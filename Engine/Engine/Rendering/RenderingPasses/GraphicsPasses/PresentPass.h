#pragma once
#include "Engine/Rendering/RenderingPasses/IRenderPass.h"

class PresentPass : public IRenderPass
{
public:
    PresentPass();

    void CreateResources() override;
    void DestroyResources() override;

    void PreExecute() override;
    void Execute(vk::CommandBuffer inCommandBuffer) override;
};
