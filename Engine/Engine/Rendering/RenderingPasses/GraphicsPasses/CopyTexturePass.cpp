#include "EnginePch.h"
#include "CopyTexturePass.h"

CopyTexturePass::CopyTexturePass(VulkanImage* inSourceTexture, VulkanImage* inTargetTexture)
    : GraphicsPass("Shaders/FullscreenVS.hlsl", "Shaders/FullscreenCopyPS.hlsl")
{
    mySourceTexture = inSourceTexture;
    myTargetTexture = inTargetTexture;
}

void CopyTexturePass::SetupAttachments()
{
    AddColorAttachment(myTargetTexture, vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore);
}

void CopyTexturePass::SetupDescriptors()
{
    // Should this be linear sampling instead? I don't think so?
    BindImage(mySourceTexture, VulkanUtils::GetSampler(SamplerMode::PointWrap), 0, vk::ShaderStageFlagBits::eFragment);
    Build();
}

void CopyTexturePass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.draw(3, 1, 0, 0);
}
