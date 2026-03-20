#include "EnginePch.h"
#include "NoDepthPass.h"

#include "Engine/Rendering/GPUResourceManager.h"
#include "Engine/Vulkan/GPUSceneSystem.h"

NoDepthPass::NoDepthPass()
: GraphicsPass("Shaders/MainVS.hlsl", "Shaders/MainPS.hlsl")
{
    
}

void NoDepthPass::SetupAttachments()
{
    AddColorAttachment(RenderSystem::Get()->myRenderTexture, vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore);
    
    AddDepthAttachment(RenderSystem::Get()->myDepthBuffer, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore);
}

void NoDepthPass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.setDepthWriteEnable(false);

    DrawToShadingBin(inCommandBuffer, EShadingBin::ShadingBin_NoDepth);
}
