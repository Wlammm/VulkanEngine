#include "EnginePch.h"
#include "FoliagePass.h"

#include "Engine/Engine.h"
#include "Engine/Rendering/FoliageSystem.h"
#include "Engine/Vulkan/VulkanBuffer.h"

// No bindless texture descriptor needed yet — phase 1 shades foliage with a flat color.
FoliagePass::FoliagePass()
    : GraphicsPass("Shaders/FoliageVS.hlsl", "Shaders/FoliagePS.hlsl", false, false)
{
}

void FoliagePass::SetupAttachments()
{
    AddColorAttachment(RenderSystem::Get()->myRenderTexture, vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedRenderTexture());

    AddDepthAttachment(RenderSystem::Get()->myDepthBuffer, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedDepthTexture());
}

void FoliagePass::PreExecute()
{
    GraphicsPass::PreExecute();

    // The cull pass wrote these via storage access; transition them for indirect-draw reads.
    FoliageSystem& foliageSystem = Engine::GetEngineSystem<FoliageSystem>();
    RegisterDynamicBufferUsage(foliageSystem.GetIndirectBuffer(), vk::PipelineStageFlagBits::eDrawIndirect, vk::AccessFlagBits::eIndirectCommandRead);
    RegisterDynamicBufferUsage(foliageSystem.GetCountBuffer(), vk::PipelineStageFlagBits::eDrawIndirect, vk::AccessFlagBits::eIndirectCommandRead);
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
