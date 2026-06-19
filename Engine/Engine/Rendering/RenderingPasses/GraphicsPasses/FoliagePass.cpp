#include "EnginePch.h"
#include "FoliagePass.h"

#include "Engine/Engine.h"
#include "Engine/Rendering/FoliageSystem.h"
#include "Engine/Vulkan/VulkanBuffer.h"

// No bindless texture descriptor needed yet — phase 1 shades foliage with a flat color.
FoliagePass::FoliagePass()
    : GraphicsPass("Shaders/FoliageVS.hlsl", "Shaders/FoliagePS.hlsl", false, false)
{
    // The cull pass writes these via storage access; declaring them here lets the
    // RenderGraph insert the indirect-read barrier automatically each frame.
    FoliageSystem& foliageSystem = Engine::GetEngineSystem<FoliageSystem>();
    RegisterIndirectDrawBuffer(foliageSystem.GetIndirectBuffer());
    RegisterIndirectDrawBuffer(foliageSystem.GetCountBuffer());
}

void FoliagePass::SetupAttachments()
{
    AddColorAttachment(RenderSystem::Get()->myRenderTexture, vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedRenderTexture());

    AddDepthAttachment(RenderSystem::Get()->myDepthBuffer, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedDepthTexture());
}

void FoliagePass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    FoliageSystem& foliageSystem = Engine::GetEngineSystem<FoliageSystem>();

    const uint maxDraws = foliageSystem.GetNumInstances();
    if (maxDraws == 0)
        return;

    inCommandBuffer.setDepthWriteEnable(true);

    inCommandBuffer.drawIndexedIndirectCount(
        foliageSystem.GetIndirectBuffer()->GetAPIResource(),
        0,
        foliageSystem.GetCountBuffer()->GetAPIResource(),
        0,
        maxDraws,
        sizeof(vk::DrawIndexedIndirectCommand));
}
