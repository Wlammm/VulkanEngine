#pragma once
#include "Engine/Rendering/RenderingPasses/ComputePass.h"

class IndirectPrePass : public ComputePass
{
public:
    IndirectPrePass();
    
    void SetupDescriptors() override;
    
    void DispatchCall(vk::CommandBuffer inCommandBuffer) override;
};
