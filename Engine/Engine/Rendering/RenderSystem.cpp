#include "EnginePch.h"
#include "RenderSystem.h"

#include "DebugPipeline.h"
#include "Engine/Engine.h"
#include "FullscreenPipeline.h"
#include "GDRPipeline.h"
#include "IndexBufferSystem.h"
#include "SkyboxPipeline.h"
#include "VertexBufferSystem.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Assets/Shader.h"
#include "Engine/Components/StaticMeshComponent.h"
#include "Engine/Core/Input.h"
#include "Engine/Systems/PointLightSystem.h"
#include "Engine/Vulkan/VulkanCommandBuffer.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanImage.h"
#include "Engine/Vulkan/VulkanImGui.h"
#include "Engine/Vulkan/VulkanPhysicalDevice.h"
#include "Engine/Vulkan/VulkanSwapChain.h"
#include "Engine/World/World.h"

RenderSystem::RenderSystem()
{
	VulkanSwapChain::OnSwapChainResized.Bind(&RenderSystem::OnSwapChainResize,this);
}

RenderSystem::~RenderSystem()
{
	VulkanSwapChain::OnSwapChainResized.UnBind(&RenderSystem::OnSwapChainResize,this);
	
	if(myDirectionalLightShadowMap)
		VulkanAllocator::DestroyImage_TS(myDirectionalLightShadowMap);
	
	// Flush out remaining upload commands as there might still be buffers waiting destruction in them.
	FlushUploadCommands();

	LOG_WARNING("RenderSubsystem::~RenderSubsystem waits for gpu idle.");
	VulkanContext::GetDevice()->waitIdle();

	DestroyRenderResources();
}

void RenderSystem::Init()
{
	myDirectionalLightShadowMap = VulkanAllocator::AllocateImage_TS("DirectionalLight ShadowMap", VulkanImage::ShadowMapCreateInfo(myShadowMapSize), VMA_MEMORY_USAGE_AUTO);
	myDirectionalLightShadowMap->CreateDepthView();
	
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
		AddGDRPass(commandBuffer);
		AddFullscreenCopyPass(commandBuffer);
	}

	commandBuffer.end();
}

vk::RenderPass& RenderSystem::GetRenderPass()
{
	return myRenderPass;
}

vk::RenderPass& RenderSystem::GetImGuiRenderPass()
{
	return myCopyToSwapchainRenderPass;
}

VulkanImage* RenderSystem::GetRenderTexture()
{
	return myRenderTexture;
}

class VulkanImage* RenderSystem::GetResolvedRenderTexture() const
{
	return myResolvedRenderTexture;
}

VulkanImage* RenderSystem::GetDepthTexture()
{
	return myDepthBuffer;
}

VulkanImage* RenderSystem::GetDirectionalLightShadowMap() const
{
	return myDirectionalLightShadowMap;
}

void RenderSystem::OnSwapChainResize()
{
	DestroyRenderResources();
	CreateRenderResources();
}

float RenderSystem::ReadDepthAtScreenPos(const glm::vec2& inNormalizedScreenPos) const
{
	glm::ivec2 imageSize = glm::ivec2(myDepthBuffer->GetSize());

	int pixelX = static_cast<int>(inNormalizedScreenPos.x * imageSize.x);
	int pixelY = static_cast<int>(inNormalizedScreenPos.y * imageSize.y);

	pixelX = std::min(pixelX, imageSize.x - 1);
	pixelY = std::min(pixelY, imageSize.y - 1);

	vk::BufferCreateInfo bufferInfo = vk::BufferCreateInfo()
		.setSize(sizeof(float))
		.setUsage(vk::BufferUsageFlagBits::eTransferDst)
		.setSharingMode(vk::SharingMode::eExclusive);

	VulkanBuffer* buffer = VulkanAllocator::AllocateBuffer_TS("Depth read buffer", bufferInfo, VMA_MEMORY_USAGE_CPU_ONLY, true);

	VulkanCommandBuffer* commandBuffer = VulkanContext::GetDevice().CreateCommandBuffer(true, false);

	vk::ImageMemoryBarrier barrier = vk::ImageMemoryBarrier()
		.setOldLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
		.setNewLayout(vk::ImageLayout::eTransferSrcOptimal)
		.setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
		.setDstAccessMask(vk::AccessFlagBits::eTransferRead)
		.setImage(myResolvedDepthTexture->GetAPIResource())
		.setSubresourceRange(vk::ImageSubresourceRange()
			.setAspectMask(vk::ImageAspectFlagBits::eDepth)
			.setBaseMipLevel(0).setLevelCount(1)
			.setBaseArrayLayer(0).setLayerCount(1));

	commandBuffer->GetAPIResource().pipelineBarrier(
		vk::PipelineStageFlagBits::eLateFragmentTests,
		vk::PipelineStageFlagBits::eTransfer,
		{}, nullptr, nullptr, barrier);

	vk::BufferImageCopy copyRegion = vk::BufferImageCopy()
		.setBufferOffset(0)
		.setBufferRowLength(0)
		.setBufferImageHeight(0)
		.setImageOffset(vk::Offset3D{pixelX, pixelY,0 })
		.setImageExtent(vk::Extent3D{1, 1, 1});
	
	copyRegion.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eDepth;
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = 1;
	
	commandBuffer->GetAPIResource().copyImageToBuffer(myResolvedDepthTexture->GetAPIResource(), vk::ImageLayout::eTransferSrcOptimal, buffer->GetAPIResource(), copyRegion);

	VulkanContext::GetDevice().FlushCommandBuffer(commandBuffer);

	float depthValue = *(float*)buffer->GetPtr();

	VulkanAllocator::DestroyBuffer_TS(buffer);
	return depthValue;
}

VulkanCommandBuffer* RenderSystem::CreateUploadCommandBuffer_TS()
{
	VulkanCommandBuffer* commandBuffer = VulkanContext::GetDevice().CreateCommandBuffer(true, true);
#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
														   .setObjectHandle(VulkanContext::GetVulkanHandle(commandBuffer->GetAPIResource()))
														   .setPObjectName("Upload command buffer")
														   .setObjectType(vk::ObjectType::eCommandBuffer));
#endif
	
	return commandBuffer;
}

void RenderSystem::QueueCommandBufferForUpload_TS(VulkanCommandBuffer* commandBuffer)
{
	std::scoped_lock lock(myUploadMutex);
	myQueuedUploadCommandBuffers.Add(commandBuffer);
}

const GDRPipeline& RenderSystem::GetGDRPipeline() const
{
	return *myGDRPipeline;
}

void RenderSystem::AddGDRPass(vk::CommandBuffer inCommandBuffer)
{
	ZoneScoped;
	{
		GPUMARK_SCOPE(inCommandBuffer, "Indirect Culling");
		myGDRPipeline->AddComputeCommands(inCommandBuffer);
	}

	VertexBufferSystem& vertexBufferSystem = Engine::GetEngineSystem<VertexBufferSystem>();
	IndexBufferSystem& indexBufferSystem = Engine::GetEngineSystem<IndexBufferSystem>();
	PointLightSystem& pointLightSystem = Engine::GetEngineSystem<PointLightSystem>();
	
	// If you've done buffer copies for vertex/index buffers before this:
	vk::BufferMemoryBarrier vertexBufferBarrier{};
	vertexBufferBarrier
		.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite) // Buffer copy writes
		.setDstAccessMask(vk::AccessFlagBits::eVertexAttributeRead) // Vertex shader reads
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setBuffer(vertexBufferSystem.GetGlobalVertexBuffer()->GetBuffer()->GetAPIResource())
		.setOffset(0)
		.setSize(VK_WHOLE_SIZE);

	vk::BufferMemoryBarrier indexBufferBarrier{};
	indexBufferBarrier
		.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite) // Buffer copy writes
		.setDstAccessMask(vk::AccessFlagBits::eIndexRead) // Vertex shader reads index buffer
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setBuffer(indexBufferSystem.GetGlobalIndexBuffer()->GetBuffer()->GetAPIResource())
		.setOffset(0)
		.setSize(VK_WHOLE_SIZE);
	
	inCommandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eTransfer, // Buffer copies stage
		vk::PipelineStageFlagBits::eVertexInput, // Vertex input stage
		{},                                      // Dependency flags
		nullptr,                                 // Memory barriers
		{ vertexBufferBarrier, indexBufferBarrier }, // Buffer barriers
		nullptr                                  // Image barriers
	);

	vk::BufferMemoryBarrier pointLightBarrier{};
	pointLightBarrier
		.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite) // Buffer copy writes
		.setDstAccessMask(vk::AccessFlagBits::eShaderRead) // Vertex shader reads index buffer
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setBuffer(indexBufferSystem.GetGlobalIndexBuffer()->GetBuffer()->GetAPIResource())
		.setOffset(0)
		.setSize(VK_WHOLE_SIZE);

	inCommandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
		vk::PipelineStageFlagBits::eFragmentShader,
		{},
		nullptr,
		pointLightBarrier,
		nullptr);

	inCommandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
			//.setRenderPass(myRenderTextureRenderPass)
			.setRenderPass(myRenderPass)
			//.setFramebuffer(myRenderTextureFrameBuffer)
			.setFramebuffer(GetVkFrameBuffer())
			.setPClearValues(myClearValues)
			.setClearValueCount(2)
			.setRenderArea(vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())))
			, vk::SubpassContents::eInline);
	{
		GPUMARK_SCOPE(inCommandBuffer, "Indirect Graphics");

		inCommandBuffer.setViewport(0, vk::Viewport()
			.setX(0)
			.setY(0)
			.setWidth(static_cast<float>(Engine::GetRenderResolution().x))
			.setHeight(static_cast<float>(Engine::GetRenderResolution().y))
			.setMinDepth(0.0f)
			.setMaxDepth(1.0f));
	
		inCommandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())));

		mySkyboxPipeline->AddGraphicsCommands(inCommandBuffer);
		myGDRPipeline->AddGraphicsCommands(inCommandBuffer);
	}

	AddDebugPass(inCommandBuffer);

	inCommandBuffer.endRenderPass();
}

void RenderSystem::AddUploadPass(vk::CommandBuffer inCommandBuffer)
{
	ZoneScoped;
	GPUMARK_SCOPE(inCommandBuffer, "UploadPass");

	std::scoped_lock lock(myUploadMutex);
	for(VulkanCommandBuffer* commandBuffer : myQueuedUploadCommandBuffers)
	{
		commandBuffer->GetAPIResource().end();
		inCommandBuffer.executeCommands(commandBuffer->GetAPIResource());
		VulkanAllocator::QueueDestroyCommand([commandBuffer]()
		{
			delete commandBuffer;
		});
	}
	myQueuedUploadCommandBuffers.Clear();
}

void RenderSystem::AddDebugPass(vk::CommandBuffer inCommandBuffer)
{
	ZoneScoped;
	GPUMARK_SCOPE(inCommandBuffer, "DebugPass");

	inCommandBuffer.setViewport(0, vk::Viewport()
								.setX(0)
								.setY(0)
								.setWidth(static_cast<float>(Engine::GetRenderResolution().x))
								.setHeight(static_cast<float>(Engine::GetRenderResolution().y))
								.setMinDepth(0.0f)
								.setMaxDepth(1.0f));

	inCommandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())));
	myDebugPipeline->AddDrawCommands(inCommandBuffer);
}

void RenderSystem::AddFullscreenCopyPass(vk::CommandBuffer inCommandBuffer)
{
	ZoneScoped;
	GPUMARK_SCOPE(inCommandBuffer, "FullscreenCopyPass");

	inCommandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
		.setRenderPass(myCopyToSwapchainRenderPass)
		.setFramebuffer(GetVkCopyToSwapchainFrameBuffer())
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

void RenderSystem::AddImGuiPass(vk::CommandBuffer inCommandBuffer)
{
	ZoneScoped;
	GPUMARK_SCOPE(inCommandBuffer, "ImGuiPass");

	inCommandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
		.setRenderPass(myCopyToSwapchainRenderPass)
		.setFramebuffer(GetVkCopyToSwapchainFrameBuffer())
		.setPClearValues(myClearValues)
		.setClearValueCount(2)
		.setRenderArea(vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())))
		, vk::SubpassContents::eInline);

	VulkanImGui::Render(inCommandBuffer);
	
	inCommandBuffer.endRenderPass();
}

void RenderSystem::FlushUploadCommands()
{
	std::scoped_lock lock(myUploadMutex);
	VulkanContext::GetDevice().FlushSecondaryCommandBuffers(myQueuedUploadCommandBuffers);
	myQueuedUploadCommandBuffers.Clear();
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
		VulkanContext::GetDevice()->destroyFramebuffer(myCopyToSwapchainFrameBuffers[i]);
	}
	myCopyToSwapchainFrameBuffers.Clear();

	VulkanContext::GetDevice()->destroyFramebuffer(myVkFrameBuffer);

	VulkanAllocator::DestroyImage_TS(myRenderTexture);
	VulkanAllocator::DestroyImage_TS(myResolvedRenderTexture);
	VulkanAllocator::DestroyImage_TS(myDepthBuffer);
	VulkanAllocator::DestroyImage_TS(myResolvedDepthTexture);
	del(myCopyPipeline);
	del(myDebugPipeline);
	del(mySkyboxPipeline);
	del(myGDRPipeline);

	VulkanContext::GetDevice()->destroyRenderPass(myRenderPass);
	VulkanContext::GetDevice()->destroyRenderPass(myCopyToSwapchainRenderPass);
}

void RenderSystem::CreatePipelines()
{
	myGDRPipeline = new GDRPipeline();
	mySkyboxPipeline = new SkyboxPipeline();
	myDebugPipeline = new DebugPipeline();
	myCopyPipeline = new FullscreenPipeline(Engine::GetAssetRegistry().GetAssetSynchronous<Shader>("FullscreenCopy.frag"), myResolvedRenderTexture, myCopyToSwapchainRenderPass);
}

void RenderSystem::CreateRenderTextures()
{
	// Multisampled render texture
	{
		vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
			.setImageType(vk::ImageType::e2D)
			.setFormat(VulkanContext::GetSwapChain().GetFormat() /*vk::Format::eR32G32B32A32Sfloat*/)
			.setExtent({ static_cast<uint>(Engine::GetRenderResolution().x), static_cast<uint>(Engine::GetRenderResolution().y), 1 })
			.setMipLevels(1)
			.setArrayLayers(1)
			.setSamples(VulkanContext::GetPhysicalDevice().GetMaxMSAASamples())
			.setTiling(vk::ImageTiling::eOptimal)
			.setUsage(vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled)
			.setSharingMode(vk::SharingMode::eExclusive)
			.setInitialLayout(vk::ImageLayout::eUndefined);

		myRenderTexture = VulkanAllocator::AllocateImage_TS("Render texture", createInfo, VMA_MEMORY_USAGE_AUTO);
		myRenderTexture->CreateView(vk::ImageViewType::e2D);
	}

	// Resolved render texture
	{
		vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
			.setImageType(vk::ImageType::e2D)
			.setFormat(VulkanContext::GetSwapChain().GetFormat() /*vk::Format::eR32G32B32A32Sfloat*/)
			.setExtent({ static_cast<uint>(Engine::GetRenderResolution().x), static_cast<uint>(Engine::GetRenderResolution().y), 1 })
			.setMipLevels(1)
			.setArrayLayers(1)
			.setSamples(vk::SampleCountFlagBits::e1)
			.setTiling(vk::ImageTiling::eOptimal)
			.setUsage(vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled)
			.setSharingMode(vk::SharingMode::eExclusive)
			.setInitialLayout(vk::ImageLayout::eUndefined);

		myResolvedRenderTexture = VulkanAllocator::AllocateImage_TS("Resolved render texture", createInfo, VMA_MEMORY_USAGE_AUTO);
		myResolvedRenderTexture->CreateView(vk::ImageViewType::e2D);
	}
	
	myDepthBuffer = VulkanAllocator::AllocateImage_TS(
		"Depth texture",
		VulkanImage::DepthCreateInfo({ VulkanContext::GetSwapChain().GetWidth(),VulkanContext::GetSwapChain().GetHeight() }, VulkanContext::GetPhysicalDevice().GetMaxMSAASamples()),
		VMA_MEMORY_USAGE_AUTO);
	myDepthBuffer->CreateDepthView();

	myResolvedDepthTexture = VulkanAllocator::AllocateImage_TS(
		"Resolved depth texture",
		VulkanImage::DepthCreateInfo({ VulkanContext::GetSwapChain().GetWidth(),VulkanContext::GetSwapChain().GetHeight() }, vk::SampleCountFlagBits::e1),
		VMA_MEMORY_USAGE_AUTO);
	myResolvedDepthTexture->CreateDepthView();
}

void RenderSystem::CreateRenderPass()
{
	{
		const std::array<vk::AttachmentDescription2KHR, 4> attachments = {
			vk::AttachmentDescription2() // Color
				.setFormat(VulkanContext::GetSwapChain().GetFormat())
				.setSamples(VulkanContext::GetPhysicalDevice().GetMaxMSAASamples())
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStoreOp(vk::AttachmentStoreOp::eStore)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal),
			vk::AttachmentDescription2() // Depth
				.setFormat(myDepthBuffer->GetFormat())
				.setSamples(VulkanContext::GetPhysicalDevice().GetMaxMSAASamples())
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal),
			vk::AttachmentDescription2() // Resolved render target
				.setFormat(VulkanContext::GetSwapChain().GetFormat())
				.setSamples(vk::SampleCountFlagBits::e1)
				.setLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStoreOp(vk::AttachmentStoreOp::eStore)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::eShaderReadOnlyOptimal),
			vk::AttachmentDescription2() // Resolved depth target
				.setFormat(myResolvedDepthTexture->GetFormat())
				.setSamples(vk::SampleCountFlagBits::e1)
				.setLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStoreOp(vk::AttachmentStoreOp::eStore)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal),
		};

		const auto colorReference = vk::AttachmentReference2().setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);
		const auto depthReference = vk::AttachmentReference2().setAttachment(1).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
		vk::AttachmentReference2 colorAttachmentResolveRef = vk::AttachmentReference2().setAttachment(2).setLayout(vk::ImageLayout::eColorAttachmentOptimal);
		vk::AttachmentReference2 depthAttachmentResolveRef = vk::AttachmentReference2().setAttachment(3).setLayout(vk::ImageLayout::eDepthAttachmentOptimal);

		List<vk::AttachmentReference2> resolveAttachments = { colorAttachmentResolveRef };

		vk::SubpassDescriptionDepthStencilResolve depthResolveInfo = vk::SubpassDescriptionDepthStencilResolve()
			.setDepthResolveMode(vk::ResolveModeFlagBits::eAverage)
			.setStencilResolveMode(vk::ResolveModeFlagBits::eNone)
			.setPDepthStencilResolveAttachment(&depthAttachmentResolveRef);
		
		const auto subpass = vk::SubpassDescription2()
			.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
			.setColorAttachments(colorReference)
			.setPDepthStencilAttachment(&depthReference)
			.setResolveAttachments(resolveAttachments)
			.setPNext(&depthResolveInfo);
		
		vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
		const std::array<vk::SubpassDependency2, 1> dependencies = {
			vk::SubpassDependency2()
				.setSrcSubpass(VK_SUBPASS_EXTERNAL)
				.setDstSubpass(0)
				.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eLateFragmentTests)
				.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
				.setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
				.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
				.setDependencyFlags(vk::DependencyFlags())
		};

		myRenderPass = VulkanContext::GetDevice()->createRenderPass2(vk::RenderPassCreateInfo2().setAttachments(attachments).setSubpasses(subpass).setDependencies(dependencies));
	}

	// Render texture pass.
	{
		const std::array<vk::AttachmentDescription, 1> attachments = {
			vk::AttachmentDescription()
				.setFormat(VulkanContext::GetSwapChain().GetFormat())
				.setSamples(vk::SampleCountFlagBits::e1)
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStoreOp(vk::AttachmentStoreOp::eStore)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::ePresentSrcKHR),
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

		myCopyToSwapchainRenderPass = VulkanContext::GetDevice()->createRenderPass(vk::RenderPassCreateInfo().setAttachments(attachments).setSubpasses(subpass).setDependencies(dependencies));
	}
}

void RenderSystem::CreateFrameBuffers()
{
	// Main pass
	{
		std::array<vk::ImageView, 4> attachments;
		attachments[0] = myRenderTexture->GetImageView();
		attachments[1] = myDepthBuffer->GetImageView();
		attachments[2] = myResolvedRenderTexture->GetImageView();
		attachments[3] = myResolvedDepthTexture->GetImageView();
		
		myVkFrameBuffer = VulkanContext::GetDevice()->createFramebuffer(vk::FramebufferCreateInfo()
			.setRenderPass(myRenderPass)
			.setAttachments(attachments)
			.setWidth(VulkanContext::GetSwapChain().GetWidth())
			.setHeight(VulkanContext::GetSwapChain().GetHeight())
			.setLayers(1));
	}

	// ImGui
	{
		std::array<vk::ImageView, 1> attachments;
		for (uint i = 0; i < VulkanContext::FrameLag; ++i)
		{
			attachments[0] = VulkanContext::GetSwapChain().GetImageView(i);
			myCopyToSwapchainFrameBuffers.Add(VulkanContext::GetDevice()->createFramebuffer(vk::FramebufferCreateInfo()
				.setRenderPass(myCopyToSwapchainRenderPass)
				.setAttachments(attachments)
				.setWidth(VulkanContext::GetSwapChain().GetWidth())
				.setHeight(VulkanContext::GetSwapChain().GetHeight())
				.setLayers(1)));
		}
	}
}

//void RenderSubsystem::UpdateFrameBuffer()
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

//void RenderSubsystem::UpdateObjectBuffer(const Transform& inTransform)
//{
//	ObjectData& buffer = myObjectData.Get();
//	buffer.myToWorld = inTransform.GetMatrix();
//}

vk::Framebuffer RenderSystem::GetVkFrameBuffer() const
{
	return myVkFrameBuffer;
}

vk::Framebuffer RenderSystem::GetVkCopyToSwapchainFrameBuffer() const
{
	return myCopyToSwapchainFrameBuffers[VulkanContext::GetSwapChain().GetFrameIndex()];
}
