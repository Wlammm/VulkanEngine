#pragma once
#include "Engine/Rendering/RenderingPasses/GraphicsPass.h"

// Draws the foliage instances using the indirect commands produced by FoliageCullPass.
class FoliagePass : public GraphicsPass
{
public:
    FoliagePass();

    void SetupAttachments() override;
    void PreExecute() override;
    void DrawCall(vk::CommandBuffer inCommandBuffer) override;
};
