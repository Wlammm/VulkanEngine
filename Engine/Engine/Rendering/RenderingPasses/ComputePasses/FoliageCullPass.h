#pragma once
#include "Engine/Rendering/RenderingPasses/ComputePass.h"

// Builds the foliage indirect draw commands and per-draw data from the compact
// instance pool. One thread per instance.
class FoliageCullPass : public ComputePass
{
public:
    FoliageCullPass();

    void DispatchCall(vk::CommandBuffer inCommandBuffer) override;
};
