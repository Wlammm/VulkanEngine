#include "EnginePch.h"
#include "MainPass.h"

#include "Engine/Rendering/GPUResourceManager.h"
#include "Engine/Vulkan/GPUSceneSystem.h"

MainPass::MainPass()
    : GraphicsPass("Shaders/MainVS.hlsl", "Shaders/MainPS.hlsl")
{
}

void MainPass::SetupAttachments()
{
    AddColorAttachment(RenderSystem::Get()->myRenderTexture, vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedRenderTexture());
    
    AddDepthAttachment(RenderSystem::Get()->myDepthBuffer, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedDepthTexture());
}

void MainPass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.setDepthWriteEnable(true);
    
    DrawToShadingBin(inCommandBuffer, EShadingBin::ShadingBin_Default);
}
