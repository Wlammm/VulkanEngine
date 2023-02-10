#include "EnginePch.h"
#include "VulkanRenderer.h"
#include "VulkanContext.h"
#include "VulkanSwapChain.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "Windows/WindowHandler.h"

VulkanRenderer::VulkanRenderer()
{
	CreatePipelines();
}

VulkanRenderer::~VulkanRenderer()
{
	VulkanContext::GetDevice()->destroyPipelineCache(myPipelineCache);
}

void VulkanRenderer::Tick()
{
	const vk::CommandBuffer& commandBuffer = VulkanContext::GetSwapChain().GetCommandBuffer();
	commandBuffer.reset();
	commandBuffer.begin(vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse));

	vk::ClearValue clearValue = vk::ClearColorValue(std::array<float, 4>({ {1.0f, 0.0f, 0.0f, 0.0f} }));

	commandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
		.setRenderPass(VulkanContext::GetSwapChain().GetRenderPass())
		.setFramebuffer(VulkanContext::GetSwapChain().GetFrameBuffer())
		.setPClearValues(&clearValue)
		.setClearValueCount(1)
		, vk::SubpassContents::eInline);



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

void VulkanRenderer::CreatePipelines()
{
	myPipelineCache = VulkanContext::GetDevice()->createPipelineCache(vk::PipelineCacheCreateInfo(), nullptr);


}
