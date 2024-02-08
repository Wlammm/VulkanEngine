#include "EnginePch.h"
#include "RenderSystem.h"
#include "Engine.h"
#include "World/World.h"
#include "Assets/Model.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanImGui.h"
#include "Vulkan/VulkanImage.h"
#include "Vulkan/VulkanAllocator.h"
#include "ECS/Components/StaticMesh.h"
#include "Assets/AssetRegistry.h"
#include "Vulkan/VulkanShader.h"
#include "Assets/Material.h"
#include "Rendering/MeshPipeline.h"
#include "Rendering/FullscreenPipeline.h"
#include "tracy/Tracy.hpp"
#include "ECS/Components/Camera.h"
#include "ECS/Components/PointLight.h"
#include "ECS/Components/Transform.h"
#include "Rendering/ShadowPipeline.h"

RenderSystem::RenderSystem()
{
	SetDependencies<const Transform, const Camera, const StaticMesh, const PointLight>();
	myInstance = this;
	SubscribeToEvent(EventType::SwapchainResized, std::bind(&RenderSystem::OnSwapChainResize, this));
}

RenderSystem::~RenderSystem()
{
	// Flush out remaining upload commands as there might still be buffers waiting destruction in them.
	FlushUploadCommands();

	LOG_WARNING("RenderSystem::~RenderSystem waits for gpu idle.");
	VulkanContext::GetDevice()->waitIdle();

	DestroyRenderResources();
	myInstance = nullptr;
}

void RenderSystem::Init()
{
	CreateRenderResources();
	VulkanImGui::Start();
}

void RenderSystem::Tick()
{
	ZoneScoped;

	const vk::CommandBuffer& commandBuffer = VulkanContext::GetSwapChain().GetCommandBuffer();
	commandBuffer.begin(vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse));

	{
		GPUMARK_SCOPE(commandBuffer, "Frame");

		AddUploadPass(commandBuffer);
		AddShadowGenerationPass(commandBuffer);
		AddMeshPass(commandBuffer);
		AddFullscreenCopyPass(commandBuffer);
	}

	commandBuffer.end();
}

vk::RenderPass& RenderSystem::GetRenderPass()
{
	return myRenderPass;
}

VulkanImage* RenderSystem::GetRenderTexture()
{
	return myRenderTexture;
}

void RenderSystem::OnSwapChainResize()
{
	DestroyRenderResources();
	CreateRenderResources();
}

void RenderSystem::AddUploadCommand_TS(void* inOwner, std::function<void(vk::CommandBuffer inCommandBuffer)> inFunction)
{
	myUploadCommandsMutex.lock();
	myUploadCommands.Add({ inOwner, inFunction });
	myUploadCommandsMutex.unlock();
}

void RenderSystem::RemoveUploadCommandsForOwner_TS(void* inOwner)
{
	myUploadCommandsMutex.lock();
	if(!myUploadCommands.IsEmpty())
	{
		for (int i = myUploadCommands.size() - 1; i >= 0; --i)
		{
			if (myUploadCommands[i].myOwner == inOwner)
				myUploadCommands.RemoveIndex(i);
		}
	}
	myUploadCommandsMutex.unlock();
}

void RenderSystem::FlushUploadCommands()
{
	vk::CommandBuffer buffer = VulkanContext::GetDevice().CreateCommandBuffer(true);
	myInstance->AddUploadPass(buffer);
	VulkanContext::GetDevice().FlushCommandBuffer(buffer);
}

const ShadowPipeline& RenderSystem::GetShadowPipeline()
{
	return *myInstance->myShadowPipeline;
}

void RenderSystem::AddUploadPass(vk::CommandBuffer inCommandBuffer)
{
	GPUMARK_SCOPE(inCommandBuffer, "UploadPass");
	myUploadCommandsMutex.lock();

	for(auto& [owner, func] : myUploadCommands)
	{
		func(inCommandBuffer);
	}
	myUploadCommands.Clear();

	myUploadCommandsMutex.unlock();
}

void RenderSystem::AddMeshPass(vk::CommandBuffer inCommandBuffer)
{
	ZoneScoped;

	GPUMARK_SCOPE(inCommandBuffer, "MeshPass");
	const auto view = Engine::GetWorld().GetRegistry().view<const Transform, const StaticMesh>();
	if (view.size_hint() == 0)
		return;

	inCommandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
		.setRenderPass(myRenderTextureRenderPass)
		.setFramebuffer(myRenderTextureFrameBuffer)
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

	myMeshPipeline->AddDrawCommands(inCommandBuffer);

	inCommandBuffer.endRenderPass();
}

void RenderSystem::AddShadowGenerationPass(vk::CommandBuffer inCommandBuffer)
{
	ZoneScoped;
	GPUMARK_SCOPE(inCommandBuffer, "ShadowMapGenerationPass");
	myShadowPipeline->AddCommands(inCommandBuffer);
}

void RenderSystem::AddFullscreenCopyPass(vk::CommandBuffer inCommandBuffer)
{
	ZoneScoped;
	GPUMARK_SCOPE(inCommandBuffer, "FullscreenCopyPass");

	// Temporary to change image format of swapchain textures.
	inCommandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
		.setRenderPass(myRenderPass)
		.setFramebuffer(GetVkFrameBuffer())
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

	myCopyPipeline->AddFullscreenPass(inCommandBuffer);

	VulkanImGui::Render(inCommandBuffer);
	inCommandBuffer.endRenderPass();

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
}

void RenderSystem::CreateRenderResources()
{
	CreateRenderTextures();
	CreateRenderPass();
	CreatePipelines();
	CreateFrameBuffers();
}

void RenderSystem::DestroyRenderResources()
{
	for (uint i = 0; i < VulkanContext::FrameLag; ++i)
	{
		VulkanContext::GetDevice()->destroyFramebuffer(mySwapchainFrameBuffers[i]);
	}
	VulkanContext::GetDevice()->destroyFramebuffer(myRenderTextureFrameBuffer);
	mySwapchainFrameBuffers.Clear();

	VulkanAllocator::DestroyImage_TS(myRenderTexture);
	VulkanAllocator::DestroyImage_TS(myDepthBuffer);
	del(myCopyPipeline);
	del(myMeshPipeline);
	del(myShadowPipeline);

	VulkanContext::GetDevice()->destroyRenderPass(myRenderPass);
	VulkanContext::GetDevice()->destroyRenderPass(myRenderTextureRenderPass);
}

void RenderSystem::CreatePipelines()
{
	myShadowPipeline = new ShadowPipeline();
	myMeshPipeline = new MeshPipeline();
	myCopyPipeline = new FullscreenPipeline(Engine::GetAssetRegistry().GetShader("FullscreenCopy.frag"), myRenderTexture);
}

void RenderSystem::CreateRenderTextures()
{
	vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
		.setImageType(vk::ImageType::e2D)
		.setFormat(VulkanContext::GetSwapChain().GetFormat() /*vk::Format::eR32G32B32A32Sfloat*/)
		.setExtent({ Engine::GetRenderResolution().x, Engine::GetRenderResolution().y, 1 })
		.setMipLevels(1)
		.setArrayLayers(1)
		.setSamples(vk::SampleCountFlagBits::e1)
		.setTiling(vk::ImageTiling::eOptimal)
		.setUsage(vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled)
		.setSharingMode(vk::SharingMode::eExclusive)
		.setInitialLayout(vk::ImageLayout::eUndefined);

	myRenderTexture = VulkanAllocator::AllocateImage_TS("Render texture", createInfo, VMA_MEMORY_USAGE_AUTO);
	myRenderTexture->CreateView(vk::ImageViewType::e2D);
	
	myDepthBuffer = VulkanAllocator::AllocateImage_TS(
		"Depth texture",
		VulkanImage::DepthCreateInfo({ VulkanContext::GetSwapChain().GetWidth(),VulkanContext::GetSwapChain().GetHeight() }),
		VMA_MEMORY_USAGE_AUTO);
	myDepthBuffer->CreateDepthView();
}

void RenderSystem::CreateRenderPass()
{
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

	// Render texture pass.
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
				.setFinalLayout(vk::ImageLayout::eShaderReadOnlyOptimal),
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

		myRenderTextureRenderPass = VulkanContext::GetDevice()->createRenderPass(vk::RenderPassCreateInfo().setAttachments(attachments).setSubpasses(subpass).setDependencies(dependencies));
	}
}

void RenderSystem::CreateFrameBuffers()
{
	std::array<vk::ImageView, 2> attachments;
	attachments[1] = myDepthBuffer->GetImageView();

	for (uint i = 0; i < VulkanContext::FrameLag; ++i)
	{
		attachments[0] = VulkanContext::GetSwapChain().GetImageView(i);
		mySwapchainFrameBuffers.Add(VulkanContext::GetDevice()->createFramebuffer(vk::FramebufferCreateInfo()
			.setRenderPass(myRenderPass)
			.setAttachments(attachments)
			.setWidth(VulkanContext::GetSwapChain().GetWidth())
			.setHeight(VulkanContext::GetSwapChain().GetHeight())
			.setLayers(1)));
	}

	// Render texture.
	{
		attachments[0] = myRenderTexture->GetImageView();
		myRenderTextureFrameBuffer = VulkanContext::GetDevice()->createFramebuffer(vk::FramebufferCreateInfo()
			.setRenderPass(myRenderTextureRenderPass)
			.setAttachments(attachments)
			.setWidth(VulkanContext::GetSwapChain().GetWidth())
			.setHeight(VulkanContext::GetSwapChain().GetHeight())
			.setLayers(1));
	}
}

//void RenderSystem::UpdateFrameBuffer()
//{
//	FrameData& buffer = myFrameData.Get();
//	auto view = Engine::GetWorld().GetRegistry().view<const Transform, const Camera>();
//
//	for (auto ent : view)
//	{
//		buffer.myProjection = view.get<const Camera>(ent).myProjection.Transposed();
//		buffer.myToView = view.get<const Transform>(ent).GetMatrix().FastInverse().Transposed();
//		return;
//	}
//
//	LOG("No render camera found.");
//}

//void RenderSystem::UpdateObjectBuffer(const Transform& inTransform)
//{
//	ObjectData& buffer = myObjectData.Get();
//	buffer.myToWorld = inTransform.GetMatrix();
//}

vk::Framebuffer RenderSystem::GetVkFrameBuffer() const
{
	return mySwapchainFrameBuffers[VulkanContext::GetSwapChain().GetFrameIndex()];
}
