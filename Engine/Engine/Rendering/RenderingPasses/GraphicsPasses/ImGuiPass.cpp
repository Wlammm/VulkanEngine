#include "EnginePch.h"
#include "ImGuiPass.h"

#include "Engine/Vulkan/VulkanImGui.h"
#include "Engine/Vulkan/VulkanSwapChain.h"

void ImGuiPass::CreateResources()
{
    {
        const std::array<vk::AttachmentDescription, 1> attachments = {
            vk::AttachmentDescription()
                .setFormat(VulkanContext::GetSwapChain().GetFormat())
                .setSamples(vk::SampleCountFlagBits::e1)
                .setLoadOp(vk::AttachmentLoadOp::eLoad)
                .setStoreOp(vk::AttachmentStoreOp::eStore)
                .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                .setInitialLayout(vk::ImageLayout::eColorAttachmentOptimal)
                .setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal),
        };
		    
        const auto colorReference = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);

        const auto subpass = vk::SubpassDescription()
            .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
            .setColorAttachments(colorReference);

        vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
        const std::array<vk::SubpassDependency, 1> dependencies = {
            vk::SubpassDependency()
                .setSrcSubpass(VK_SUBPASS_EXTERNAL)
                .setDstSubpass(0)
                .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                .setSrcAccessMask(vk::AccessFlagBits())
                .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead)
                .setDependencyFlags(vk::DependencyFlags()),
        };

        myRenderPass = VulkanContext::GetDevice()->createRenderPass(vk::RenderPassCreateInfo().setAttachments(attachments).setSubpasses(subpass).setDependencies(dependencies));
    }
    
    std::array<vk::ImageView, 1> attachments;
    for (uint i = 0; i < VulkanContext::FrameLag; ++i)
    {
        attachments[0] = VulkanContext::GetSwapChain().GetImageView(i);
        myFrameBuffers.Add(VulkanContext::GetDevice()->createFramebuffer(vk::FramebufferCreateInfo()
            .setRenderPass(myRenderPass)
            .setAttachments(attachments)
            .setWidth(VulkanContext::GetSwapChain().GetWidth())
            .setHeight(VulkanContext::GetSwapChain().GetHeight())
            .setLayers(1)));
    }
    
    DO_ONCE(    
        VulkanImGui::Start(myRenderPass); 
    );
}

void ImGuiPass::DestroyResources()
{
    VulkanContext::GetDevice()->destroyRenderPass(myRenderPass);
    
    for (uint i = 0; i < VulkanContext::FrameLag; ++i)
    {
        VulkanContext::GetDevice()->destroyFramebuffer(myFrameBuffers[i]);
    }
    myFrameBuffers.Clear();
}

void ImGuiPass::Execute(vk::CommandBuffer inCommandBuffer)
{
    GPUMARK_SCOPE(inCommandBuffer, "ImGuiPass");
    
    vk::ClearValue myClearValues[2] = {
        vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} })),
        vk::ClearDepthStencilValue(1.0f, 0u) };
    
    inCommandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
        .setRenderPass(myRenderPass)
        .setFramebuffer(myFrameBuffers[VulkanContext::GetSwapChain().GetFrameIndex()])
        .setPClearValues(myClearValues)
        .setClearValueCount(2)
        .setRenderArea(vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())))
        , vk::SubpassContents::eInline);
    
    inCommandBuffer.setViewport(0, vk::Viewport()
        .setX(0)
        .setY(0)
        .setWidth(static_cast<float>(Engine::GetRenderResolution().x))
        .setHeight(static_cast<float>(Engine::GetRenderResolution().y))
        .setMinDepth(0.0f)
        .setMaxDepth(1.0f));
    inCommandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())));
    
    VulkanImGui::Render(inCommandBuffer);
    
    inCommandBuffer.endRenderPass();
}
