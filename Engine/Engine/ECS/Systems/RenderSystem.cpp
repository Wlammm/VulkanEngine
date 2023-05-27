#include "EnginePch.h"
#include "RenderSystem.h"
#include "Engine.h"
#include "World/World.h"
#include "Assets/Model.h"
#include "Rendering/Mesh.h"
#include "Vulkan/VulkanPipeline.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanTexture.h"
#include "Vulkan/VulkanImGui.h"
#include "Vulkan/VulkanImage.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanDepthBuffer.h"

RenderSystem::RenderSystem()
{
	Model::CreateInfo createInfo{};
	createInfo.InvertY = true;
	myModel = new Model("Assets/Tree.fbx", createInfo);
	myTexture = new VulkanTexture("Assets/Bark.tga", SamplerMode::Wrap);

	myDepthBuffer = new VulkanDepthBuffer({ VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight() });

	CreateRenderPass();
	CreateFrameBuffer();
	CreatePipelines();
}

RenderSystem::~RenderSystem()
{
	LOG_WARNING("RenderSystem::~RenderSystem waits for gpu idle.");
	VulkanContext::GetDevice()->waitIdle();
	for(uint i = 0; i < VulkanContext::GetSwapChain().GetFrameLag(); ++i)
	{
		VulkanContext::GetDevice()->destroyFramebuffer(myFrameBuffers[i]);
	}
	myFrameBuffers.Clear();

	del(myDepthBuffer);
	del(myTexture);
	del(myModel);
	del(myPipeline);

	VulkanContext::GetDevice()->destroyRenderPass(myRenderPass);
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
		.setRenderPass(myRenderPass)
		.setFramebuffer(GetFrameBuffer())
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

vk::RenderPass& RenderSystem::GetRenderPass()
{
	return myRenderPass;
}

void RenderSystem::CreatePipelines()
{
	{
		VulkanPipeline::CreateInfo createInfo;
		createInfo.VertexShaderPath = "../Engine/Engine/Shaders/VertexShader.spv";
		createInfo.FragmentShaderPath = "../Engine/Engine/Shaders/FragmentShader.spv";
		createInfo.RenderPass = myRenderPass;
		createInfo.UniformBuffers = { &myFrameData, &myObjectData };
		createInfo.Textures = { myTexture };
		myPipeline = new VulkanPipeline(createInfo);
	}
	
	//{
	//	vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
	//		.setImageType(vk::ImageType::e2D)
	//		.setFormat(vk::Format::eR32G32B32A32Sfloat)
	//		.setExtent({ Engine::GetRenderResolution().x, Engine::GetRenderResolution().y, 1 })
	//		.setMipLevels(1)
	//		.setArrayLayers(1)
	//		.setSamples(vk::SampleCountFlagBits::e1)
	//		.setTiling(vk::ImageTiling::eOptimal)
	//		.setUsage(vk::ImageUsageFlagBits::eColorAttachment)
	//		.setSharingMode(vk::SharingMode::eExclusive)
	//		.setInitialLayout(vk::ImageLayout::eUndefined);
	//
	//	myRenderTexture = VulkanContext::GetAllocator().AllocateImage("Render texture", createInfo, VMA_MEMORY_USAGE_GPU_ONLY);
	//	myRenderTexture->CreateView(vk::ImageViewType::e2D, vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
	//}
}

void RenderSystem::CreateRenderPass()
{
	const std::array<vk::AttachmentDescription, 2> attachments = {
		vk::AttachmentDescription()
			.setFormat(VulkanContext::GetSwapChain().GetFormat())
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eStore)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::ePresentSrcKHR),
		vk::AttachmentDescription()
			.setFormat(myDepthBuffer->GetFormat())
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal),
	};

	const auto colorReference = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);
	const auto depthReference = vk::AttachmentReference().setAttachment(1).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

	const auto subpass = vk::SubpassDescription()
		.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
		.setColorAttachments(colorReference)
		.setPDepthStencilAttachment(&depthReference);

	vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	const std::array<vk::SubpassDependency, 2> dependencies = {
		vk::SubpassDependency()  // Depth buffer is shared between swapchain images
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(stages)
			.setDstStageMask(stages)
			.setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
			.setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
			.setDependencyFlags(vk::DependencyFlags()),
		vk::SubpassDependency()
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setSrcAccessMask(vk::AccessFlagBits())
			.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eColorAttachmentRead)
			.setDependencyFlags(vk::DependencyFlags())
	};

	myRenderPass = VulkanContext::GetDevice()->createRenderPass(vk::RenderPassCreateInfo().setAttachments(attachments).setSubpasses(subpass).setDependencies(dependencies));
}

void RenderSystem::CreateFrameBuffer()
{
	std::array<vk::ImageView, 2> attachments;
	attachments[1] = myDepthBuffer->GetImageView();

	for (uint i = 0; i < VulkanContext::GetSwapChain().GetFrameLag(); ++i)
	{
		attachments[0] = VulkanContext::GetSwapChain().GetImageView(i);
		myFrameBuffers.Add(VulkanContext::GetDevice()->createFramebuffer(vk::FramebufferCreateInfo()
			.setRenderPass(myRenderPass)
			.setAttachments(attachments)
			.setWidth(VulkanContext::GetSwapChain().GetWidth())
			.setHeight(VulkanContext::GetSwapChain().GetHeight())
			.setLayers(1)));
	}
}

void RenderSystem::UpdateFrameBuffer()
{
	FrameData& buffer = myFrameData.Get();
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
	ObjectData& buffer = myObjectData.Get();
	buffer.myToWorld = inTransform.GetMatrix();
}

vk::Framebuffer RenderSystem::GetFrameBuffer() const
{
	return myFrameBuffers[VulkanContext::GetSwapChain().GetSwapChainIndex()];
}
