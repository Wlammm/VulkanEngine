#include "EnginePch.h"
#include "PrePass.h"

PrePass::PrePass()
    : GraphicsPass("Shaders/MainVS.hlsl", "")
{
}

void PrePass::SetupAttachments()
{
    AddDepthAttachment(RenderSystem::Get()->myDepthBuffer, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedDepthTexture());
}

void PrePass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.setDepthWriteEnable(true);
    
    DrawFromShadingBin(inCommandBuffer, EShadingBin::ShadingBin_Default);
}
