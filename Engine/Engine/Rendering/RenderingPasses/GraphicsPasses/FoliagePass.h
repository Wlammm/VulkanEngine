#pragma once
#include "Engine/Rendering/RenderingPasses/GraphicsPass.h"

// Draws procedural grass blades using the per-LOD indirect commands produced by
// FoliageCullPass. Blades are generated in the vertex shader from SV_VertexID (no
// vertex/index streams), so this pass declares an empty vertex input, a triangle-strip
// topology and disables back-face culling (blades are two-sided).
class FoliagePass : public GraphicsPass
{
public:
    FoliagePass();

    void SetupAttachments() override;
    void DrawCall(vk::CommandBuffer inCommandBuffer) override;

    List<vk::VertexInputBindingDescription>& GetVertexBindingDescriptor() const override;
    List<vk::VertexInputAttributeDescription>& GetVertexAttributeDescriptions() const override;
    vk::PrimitiveTopology GetPrimitiveTopology() const override;
    vk::CullModeFlags GetCullMode() const override;
};
