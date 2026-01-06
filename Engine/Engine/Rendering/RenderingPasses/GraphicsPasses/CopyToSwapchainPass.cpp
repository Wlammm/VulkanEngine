#include "EnginePch.h"
#include "CopyToSwapchainPass.h"

#include "Engine/Vulkan/VulkanSwapChain.h"

CopyToSwapchainPass::CopyToSwapchainPass(VulkanImage* inSourceImage)
    : GraphicsPass("Shaders/FullscreenVS.hlsl", "Shaders/FullscreenCopyPS.hlsl", true)
{
    mySourceTexture = inSourceImage;
}

void CopyToSwapchainPass::SetupDescriptors()
{
    // Should this be linear sampling instead? I don't think so?
    BindImage(mySourceTexture, VulkanUtils::GetSampler(SamplerMode::PointWrap), 0, vk::ShaderStageFlagBits::eFragment, vk::ImageLayout::eShaderReadOnlyOptimal);
    Build();
}

void CopyToSwapchainPass::SetupAttachments()
{
    RegisterDynamicColorAttachment(VulkanContext::GetSwapChain().GetFormat(), vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eStore);
}

vk::SampleCountFlagBits CopyToSwapchainPass::GetNumSamples() const
{
    return vk::SampleCountFlagBits::e1;
}

void CopyToSwapchainPass::GetDynamicColorAttachments()
{
    VulkanImage* image = VulkanContext::GetSwapChain().GetImage(VulkanContext::GetSwapChain().GetFrameIndex());
    
    AddDynamicColorAttachment(image, vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore);
}

void CopyToSwapchainPass::GetDynamicDepthAttachments()
{ }

void CopyToSwapchainPass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.draw(3, 1, 0, 0);
}
