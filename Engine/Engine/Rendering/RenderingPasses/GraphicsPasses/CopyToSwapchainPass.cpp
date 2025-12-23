#include "EnginePch.h"
#include "CopyToSwapchainPass.h"

#include "Engine/Vulkan/VulkanSwapChain.h"

CopyToSwapchainPass::CopyToSwapchainPass(VulkanImage* inSourceImage)
    : GraphicsPass("Shaders/FullscreenVS.vert", "Shaders/FullscreenCopy.frag", true)
{
    mySourceTexture = inSourceImage;
}

void CopyToSwapchainPass::SetupDescriptors()
{
    // Should this be linear sampling instead? I don't think so?
    myDescriptorSet.BindImage(mySourceTexture, VulkanUtils::GetSampler(SamplerMode::PointWrap), 0, vk::ShaderStageFlagBits::eFragment);
    myDescriptorSet.Build();
}

void CopyToSwapchainPass::SetupAttachments()
{
    AddDynamicColorAttachment(VulkanContext::GetSwapChain().GetFormat(), vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eStore);
    // We use dynamic attachments so do nothing here.
}

vk::SampleCountFlagBits CopyToSwapchainPass::GetNumSamples() const
{
    return vk::SampleCountFlagBits::e1;
}

List<vk::RenderingAttachmentInfo> CopyToSwapchainPass::GetDynamicColorAttachments()
{
    return { vk::RenderingAttachmentInfo().setLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setImageLayout(vk::ImageLayout::eAttachmentOptimal)
            .setImageView(VulkanContext::GetSwapChain().GetImageView(VulkanContext::GetSwapChain().GetFrameIndex()))
            .setClearValue(vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} }))) };
}

vk::RenderingAttachmentInfo CopyToSwapchainPass::GetDynamicDepthAttachments()
{
    return vk::RenderingAttachmentInfo();
}

void CopyToSwapchainPass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.draw(3, 1, 0, 0);
}
