#pragma once
#include "Engine/Rendering/RenderingPasses/ComputePass.h"

class IndirectCullPass : public ComputePass
{
public:
    IndirectCullPass();
    
    void SetupDescriptors() override;
    
    void DispatchCall(vk::CommandBuffer inCommandBuffer) override;
    
private:
    
};
