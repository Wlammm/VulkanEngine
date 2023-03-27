#include "EnginePch.h"
#include "RenderSystem.h"
#include "Vulkan/VulkanPipeline.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Engine.h"
#include "World/World.h"
#include "Assets/Model.h"
#include "Rendering/Mesh.h"
#include "Vulkan/VulkanTexture.h"
#include "Vulkan/VulkanImGui.h"
#include "Vulkan/VulkanImage.h"
#include "Vulkan/VulkanAllocator.h"

RenderSystem::RenderSystem()
{
	Model::CreateInfo createInfo{};
	createInfo.InvertY = true;
	myModel = new Model("Assets/Tree.fbx", createInfo);
	myTexture = new VulkanTexture("Assets/Bark.tga", SamplerMode::Wrap);

	CreatePipelines();
}

RenderSystem::~RenderSystem()
{
	del(myTexture);
	del(myModel);
	del(myPipeline);
}

void RenderSystem::Tick()
{
	UpdateFrameBuffer();

	const vk::CommandBuffer& commandBuffer = VulkanContext::GetSwapChain().GetCommandBuffer();
	commandBuffer.begin(vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse));

	vk::ClearValue clearValues[2] = { 
		vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} })), 
		vk::ClearDepthStencilValue(1.0f, 0u) };

	commandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
		.setRenderPass(VulkanContext::GetSwapChain().GetRenderPass())
		.setFramebuffer(VulkanContext::GetSwapChain().GetFrameBuffer())
		.setPClearValues(clearValues)
		.setClearValueCount(2)
		.setRenderArea(vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())))
		, vk::SubpassContents::eInline);

	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline->GetPipeline());
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipeline->GetPipelineLayout(), 0, myPipeline->GetDescriptorSet(), {});

	commandBuffer.setViewport(0, vk::Viewport()
		.setX(0)
		.setY(0)
		.setWidth(static_cast<float>(Engine::GetRenderResolution().x))
		.setHeight(static_cast<float>(Engine::GetRenderResolution().y))
		.setMinDepth(0.0f)
		.setMaxDepth(1.0f));

	commandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())));

	UpdateObjectBuffer(Transform());

	for(const Mesh& mesh : myModel->GetMeshes())
	{
		mesh.Bind(commandBuffer);
		commandBuffer.drawIndexed(mesh.NumIndices, 1, 0, 0, 0);
	}

	VulkanImGui::Render(commandBuffer);
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
	{
		VulkanPipeline::CreateInfo createInfo;
		createInfo.VertexShaderPath = "../Engine/Engine/Shaders/VertexShader.spv";
		createInfo.FragmentShaderPath = "../Engine/Engine/Shaders/FragmentShader.spv";
		createInfo.RenderPass = VulkanContext::GetSwapChain().GetRenderPass();
		createInfo.UniformBuffers = { &myFrameBuffer, &myObjectBuffer };
		createInfo.Textures = { myTexture };
		myPipeline = new VulkanPipeline(createInfo);
	}
	
	{
		vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
			.setMipLevels(1)
			.setArrayLayers(1)
			.setFormat(vk::Format::eR32G32B32Sfloat)
			.setImageType(vk::ImageType::e2D)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setExtent({ Engine::GetRenderResolution().x, Engine::GetRenderResolution().y, 1 });
		myRenderTexture = VulkanContext::GetAllocator().AllocateImage("Render texture", createInfo, VMA_MEMORY_USAGE_GPU_ONLY);
		myRenderTexture->CreateView(vk::ImageViewType::e2D, vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
	}
}

void RenderSystem::UpdateFrameBuffer()
{
	FrameBuffer& buffer = myFrameBuffer.Get();
	auto view = Engine::GetWorld().GetRegistry().view<const Transform, const Camera>();

	for (auto ent : view)
	{
		buffer.myProjection = view.get<const Camera>(ent).myProjection.Transposed();
		buffer.myToView = view.get<const Transform>(ent).GetMatrix().FastInverse().Transposed();
		return;
	}

	LOG("No render camera found.");
}

void RenderSystem::UpdateObjectBuffer(const Transform& inTransform)
{
	ObjectBuffer& buffer = myObjectBuffer.Get();
	buffer.myToWorld = inTransform.GetMatrix();
}
