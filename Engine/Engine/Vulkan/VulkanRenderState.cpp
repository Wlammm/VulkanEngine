#include "EnginePch.h"
#include "VulkanRenderState.h"
#include "VulkanImage.h"
#include "VulkanDepthBuffer.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderImages.h"
#include "Engine.h"

VulkanRenderState::VulkanRenderState(const CreateInfo& inCreateInfo)
{
	CreateRenderPass(inCreateInfo);
	CreateFramebuffer(inCreateInfo);
}

void VulkanRenderState::Begin(vk::CommandBuffer inCommandBuffer)
{
	vk::ClearValue clearValues[2] = {
	vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} })),
	vk::ClearDepthStencilValue(1.0f, 0u) };

	inCommandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
		.setRenderPass(myRenderPass)
		.setFramebuffer(myFrameBuffers[VulkanContext::GetSwapChain().GetSwapChainIndex()])
		.setPClearValues(clearValues)
		.setClearValueCount(2)
		.setRenderArea(vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())))
		, vk::SubpassContents::eInline);

}

void VulkanRenderState::End(vk::CommandBuffer inCommandBuffer)
{

}

void VulkanRenderState::CreateRenderPass(const CreateInfo& inCreateInfo)
{
	List<vk::AttachmentDescription> attachments = {
		vk::AttachmentDescription()
			.setFormat(inCreateInfo.ColorTargets->GetFormat())
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eStore)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::ePresentSrcKHR),
	};
	const vk::AttachmentReference colorReference = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);

	vk::SubpassDescription subpass = vk::SubpassDescription()
		.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
		.setColorAttachments(colorReference);

	List<vk::SubpassDependency> dependencies = {
		vk::SubpassDependency()
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setSrcAccessMask(vk::AccessFlagBits())
			.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead)
			.setDependencyFlags(vk::DependencyFlags()),
	};

	if(inCreateInfo.DepthBuffer)
	{
		attachments.Add(vk::AttachmentDescription()
			.setFormat(inCreateInfo.DepthBuffer->GetFormat())
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal));

		const auto depthReference = vk::AttachmentReference().setAttachment(1).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
		subpass.setPDepthStencilAttachment(&depthReference);

		const vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
		dependencies.Add(vk::SubpassDependency()  // Depth buffer is shared between swapchain images
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(stages)
			.setDstStageMask(stages)
			.setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
			.setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
			.setDependencyFlags(vk::DependencyFlags()));
	}

	myRenderPass = VulkanContext::GetDevice()->createRenderPass(vk::RenderPassCreateInfo().setAttachments(attachments).setSubpasses(subpass).setDependencies(dependencies));
}

void VulkanRenderState::CreateFramebuffer(const CreateInfo& inCreateInfo)
{
	List<vk::ImageView> attachments;
	attachments.Reserve(inCreateInfo.DepthBuffer ? 2 : 1);
	attachments[1] = inCreateInfo.DepthBuffer->GetImageView();

	for (int i = 0; i < VulkanContext::GetSwapChain().GetFrameLag(); ++i)
	{
		attachments[0] = inCreateInfo.ColorTargets->GetView(i);
		myFrameBuffers.Add(VulkanContext::GetDevice()->createFramebuffer(vk::FramebufferCreateInfo()
			.setRenderPass(myRenderPass)
			.setAttachments(attachments)
			.setWidth(Engine::GetRenderResolution().x)
			.setHeight(Engine::GetRenderResolution().y)
			.setLayers(1)));
	}
}
