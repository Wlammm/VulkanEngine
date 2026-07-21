#include "EnginePch.h"
#include "FoliagePass.h"

#include "Engine/Engine.h"
#include "Engine/Rendering/FoliageSystem.h"
#include "Engine/Shaders/Shared/FoliageStructs.hpp"
#include "Engine/Vulkan/VulkanBuffer.h"

namespace
{
    // Must match BladeDrawConstants in FoliageVS.hlsl.
    struct BladeDrawConstants
    {
        uint mySegments;
        uint myInstanceOffset;
    };
}

FoliagePass::FoliagePass()
    : GraphicsPass("Shaders/FoliageVS.hlsl", "Shaders/FoliagePS.hlsl", false, false)
{
    // The cull pass writes the indirect commands via storage access; declaring the
    // buffer here lets the RenderGraph insert the indirect-read barrier each frame.
    FoliageSystem& foliageSystem = Engine::GetEngineSystem<FoliageSystem>();
    RegisterIndirectDrawBuffer(foliageSystem.GetIndirectBuffer());
}

void FoliagePass::SetupAttachments()
{
    AddColorAttachment(RenderSystem::Get()->myRenderTexture, vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedRenderTexture());

    AddDepthAttachment(RenderSystem::Get()->myDepthBuffer, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedDepthTexture());
}

void FoliagePass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    FoliageSystem& foliageSystem = Engine::GetEngineSystem<FoliageSystem>();

    if (foliageSystem.GetNumInstances() == 0)
        return;

    inCommandBuffer.setDepthWriteEnable(true);

    VulkanBuffer* indirectBuffer = foliageSystem.GetIndirectBuffer();
    const uint binCapacity = foliageSystem.GetCapacity();

    // One instanced draw per LOD bin. Each bin's blades occupy a contiguous region of
    // the blade buffer at (lod * binCapacity); the push constant carries that offset and
    // the bin's segment count, while the indirect command carries the accumulated
    // instanceCount plus the per-blade vertexCount written by the pre-pass.
    for (uint lod = 0; lod < FOLIAGE_LOD_COUNT; ++lod)
    {
        BladeDrawConstants constants;
        constants.mySegments = FOLIAGE_LOD_SEGMENTS[lod];
        constants.myInstanceOffset = lod * binCapacity;
        inCommandBuffer.pushConstants(myPipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(BladeDrawConstants), &constants);

        inCommandBuffer.drawIndirect(
            indirectBuffer->GetAPIResource(),
            lod * sizeof(FoliageDrawCommand),
            1,
            sizeof(FoliageDrawCommand));
    }
}

List<vk::VertexInputBindingDescription>& FoliagePass::GetVertexBindingDescriptor() const
{
    // Procedural blades: no vertex streams.
    static List<vk::VertexInputBindingDescription> empty;
    return empty;
}

List<vk::VertexInputAttributeDescription>& FoliagePass::GetVertexAttributeDescriptions() const
{
    static List<vk::VertexInputAttributeDescription> empty;
    return empty;
}

vk::PrimitiveTopology FoliagePass::GetPrimitiveTopology() const
{
    return vk::PrimitiveTopology::eTriangleStrip;
}

vk::CullModeFlags FoliagePass::GetCullMode() const
{
    return vk::CullModeFlagBits::eNone; // blades are two-sided
}
