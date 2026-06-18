#pragma once
#include "Engine/Rendering/RenderingPasses/IRenderPass.h"

// Clears the foliage draw counter to 0 before the foliage cull pass fills it.
class FoliagePrePass : public IRenderPass
{
public:
    void PreExecute() override;

    void CreateResources() override {}
    void DestroyResources() override {}

    void Execute(vk::CommandBuffer inCommandBuffer) override;
};
