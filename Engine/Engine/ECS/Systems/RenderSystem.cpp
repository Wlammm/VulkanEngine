#include "EnginePch.h"
#include "RenderSystem.h"
#include "Vulkan/VulkanPipeline.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanSwapChain.h"

RenderSystem::RenderSystem()
{
	CreatePipelines();
}

RenderSystem::~RenderSystem()
{
	del(myPipeline);
}

void RenderSystem::Tick()
{
	const vk::CommandBuffer& commandBuffer = VulkanContext::GetSwapChain().GetCommandBuffer();
	commandBuffer.begin(vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse));

	vk::ClearValue clearValue = vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} }));

	commandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
		.setRenderPass(VulkanContext::GetSwapChain().GetRenderPass())
		.setFramebuffer(VulkanContext::GetSwapChain().GetFrameBuffer())
		.setPClearValues(&clearValue)
		.setClearValueCount(1)
		.setRenderArea(vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())))
		, vk::SubpassContents::eInline);

	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline->GetPipeline());
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipeline->GetPipelineLayout(), 0, myPipeline->GetDescriptorSet(), {});

	float viewport_dimension;
	float viewport_x = 0.0f;
	float viewport_y = 0.0f;
	if (VulkanContext::GetSwapChain().GetWidth() < VulkanContext::GetSwapChain().GetHeight())
	{
		viewport_dimension = static_cast<float>(VulkanContext::GetSwapChain().GetWidth());
		viewport_y = (VulkanContext::GetSwapChain().GetHeight() - VulkanContext::GetSwapChain().GetWidth()) / 2.0f;
	}
	else
	{
		viewport_dimension = static_cast<float>(VulkanContext::GetSwapChain().GetHeight());
		viewport_x = (VulkanContext::GetSwapChain().GetWidth() - VulkanContext::GetSwapChain().GetHeight()) / 2.0f;
	}

	commandBuffer.setViewport(0, vk::Viewport()
		.setX(viewport_x)
		.setY(viewport_y)
		.setWidth(viewport_dimension)
		.setHeight(viewport_dimension)
		.setMinDepth(0.0f)
		.setMaxDepth(1.0f));

	commandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())));
	commandBuffer.draw(3, 1, 0, 0);

	commandBuffer.endRenderPass();

	/*commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eBottomOfPipe, vk::PipelineStageFlagBits::eBottomOfPipe, vk::DependencyFlagBits(), {}, {},
		vk::ImageMemoryBarrier()
		.setSrcAccessMask(vk::AccessFlags())
		.setDstAccessMask(vk::AccessFlags())
		.setOldLayout(vk::ImageLayout::ePresentSrcKHR)
		.setNewLayout(vk::ImageLayout::ePresentSrcKHR)
		.setSrcQueueFamilyIndex(VulkanContext::GetPhysicalDevice().GetGraphicsQueueIndex())
		.setDstQueueFamilyIndex(VulkanContext::GetPhysicalDevice().GetPresentQueueIndex())
		.setImage(VulkanContext::GetSwapChain().GetImage())
		.setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)));*/

	commandBuffer.end();
}

void RenderSystem::CreatePipelines()
{
	VulkanPipeline::CreateInfo createInfo;
	createInfo.VertexShaderPath = "../Engine/Engine/Shaders/VertexShader.spv";
	createInfo.FragmentShaderPath = "../Engine/Engine/Shaders/FragmentShader.spv";
	createInfo.RenderPass = VulkanContext::GetSwapChain().GetRenderPass();
	myPipeline = new VulkanPipeline(createInfo);
}
