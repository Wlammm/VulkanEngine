#pragma once
#include "Engine/Rendering/RenderingPasses/ComputePass.h"

class IndirectPrePass : public IRenderPass
{
public:
    void PreExecute() override;
    
    void CreateResources() override {}
    void DestroyResources() override {}
    
    void Execute(vk::CommandBuffer inCommandBuffer) override;
};
