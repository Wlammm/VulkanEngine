#pragma once
#include "Engine/Rendering/RenderingPasses/ComputePass.h"

class IndirectCullingComputePass : public ComputePass
{
public:
    IndirectCullingComputePass();
    
    void SetupDescriptors() override;
    
    void DispatchCall(vk::CommandBuffer inCommandBuffer) override;
};
