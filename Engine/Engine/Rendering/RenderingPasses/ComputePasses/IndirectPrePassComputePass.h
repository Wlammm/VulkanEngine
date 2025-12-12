#pragma once
#include "Engine/Rendering/RenderingPasses/ComputePass.h"

class IndirectPrePassComputePass : public ComputePass
{
public:
    IndirectPrePassComputePass();
    
    void SetupDescriptors() override;
    
    void DispatchCall(vk::CommandBuffer inCommandBuffer) override;
};
