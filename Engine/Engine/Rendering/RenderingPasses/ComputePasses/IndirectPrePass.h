#pragma once
#include "Engine/Rendering/RenderingPasses/ComputePass.h"

class IndirectPrePass : public IRenderPass
{
public:
    IndirectPrePass();

    void CreateResources() override;
    void DestroyResources() override;
    
    
    
    void Execute(vk::CommandBuffer inCommandBuffer) override;
};
