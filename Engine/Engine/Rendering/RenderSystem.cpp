#include "EnginePch.h"
#include "RenderSystem.h"

#include "DebugPipeline.h"
#include "Engine.h"
#include "FullscreenPipeline.h"
#include "GDRPipeline.h"
#include "IndexBufferSystem.h"
#include "VertexBufferSystem.h"
#include "AssetRegistry/AssetRegistry.h"
#include "Assets/Shader.h"
#include "Components/StaticMeshComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "Core/Input.h"
#include "Systems/PointLightSystem.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanImage.h"
#include "Vulkan/VulkanImGui.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanSwapChain.h"
#include "World/World.h"

RenderSystem::RenderSystem()
{
	SubscribeToEvent(EventType::SwapchainResized, std::bind(&RenderSystem::OnSwapChainResize, this));
}

RenderSystem::~RenderSystem()
{
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

vk::CommandBuffer RenderSystem::CreateUploadCommandBuffer_TS()
{
	vk::CommandBuffer commandBuffer = VulkanContext::GetDevice().CreateCommandBufferForThread(std::this_thread::get_id(), true, true);
	
	std::scoped_lock lock(myUploadMutex);
	myUploadCommandBuffersToThreadIDLUT.insert({commandBuffer, std::this_thread::get_id()});
	return commandBuffer;
}

void RenderSystem::QueueCommandBufferForUpload_TS(vk::CommandBuffer commandBuffer)
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
	for(vk::CommandBuffer commandBuffer : myQueuedUploadCommandBuffers)
	{
		commandBuffer.end();
		inCommandBuffer.executeCommands(commandBuffer);
		std::thread::id threadId = myUploadCommandBuffersToThreadIDLUT.at(commandBuffer);
		myUploadCommandBuffersToThreadIDLUT.erase(commandBuffer);
		
		VulkanAllocator::QueueDestroyCommand([threadId, commandBuffer]()
		{
			VulkanContext::GetDevice()->freeCommandBuffers(VulkanContext::GetDevice().GetCommandPoolForThread(threadId), commandBuffer);
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
	for(vk::CommandBuffer commandBuffer : myQueuedUploadCommandBuffers)
	{
		std::thread::id threadId = myUploadCommandBuffersToThreadIDLUT.at(commandBuffer);
		myUploadCommandBuffersToThreadIDLUT.erase(commandBuffer);
		VulkanContext::GetDevice().FlushCommandBufferFromThread(threadId, commandBuffer);
	}
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
	del(myCopyPipeline);
	del(myDebugPipeline);
	del(myGDRPipeline);

	VulkanContext::GetDevice()->destroyRenderPass(myRenderPass);
	VulkanContext::GetDevice()->destroyRenderPass(myCopyToSwapchainRenderPass);
}

void RenderSystem::CreatePipelines()
{
	myGDRPipeline = new GDRPipeline();
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
}

void RenderSystem::CreateRenderPass()
{
	{
		const std::array<vk::AttachmentDescription, 3> attachments = {
			vk::AttachmentDescription()
				.setFormat(VulkanContext::GetSwapChain().GetFormat())
				.setSamples(VulkanContext::GetPhysicalDevice().GetMaxMSAASamples())
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStoreOp(vk::AttachmentStoreOp::eStore)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal),
			vk::AttachmentDescription() // Depth
				.setFormat(myDepthBuffer->GetFormat())
				.setSamples(VulkanContext::GetPhysicalDevice().GetMaxMSAASamples())
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal),
			vk::AttachmentDescription()
				.setFormat(VulkanContext::GetSwapChain().GetFormat())
				.setSamples(vk::SampleCountFlagBits::e1)
				.setLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStoreOp(vk::AttachmentStoreOp::eStore)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::eShaderReadOnlyOptimal),
		};

		const auto colorReference = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);
		const auto depthReference = vk::AttachmentReference().setAttachment(1).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
		vk::AttachmentReference colorAttachmentResolveRef = vk::AttachmentReference().setAttachment(2).setLayout(vk::ImageLayout::eColorAttachmentOptimal);
		
		const auto subpass = vk::SubpassDescription()
			.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
			.setColorAttachments(colorReference)
			.setPDepthStencilAttachment(&depthReference)
			.setResolveAttachments(colorAttachmentResolveRef);

		vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
		const std::array<vk::SubpassDependency, 1> dependencies = {
			vk::SubpassDependency()
				.setSrcSubpass(VK_SUBPASS_EXTERNAL)
				.setDstSubpass(0)
				.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eLateFragmentTests)
				.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
				.setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
				.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
				.setDependencyFlags(vk::DependencyFlags())
		};

		myRenderPass = VulkanContext::GetDevice()->createRenderPass(vk::RenderPassCreateInfo().setAttachments(attachments).setSubpasses(subpass).setDependencies(dependencies));
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
		std::array<vk::ImageView, 3> attachments;
		attachments[0] = myRenderTexture->GetImageView();
		attachments[1] = myDepthBuffer->GetImageView();
		attachments[2] = myResolvedRenderTexture->GetImageView();
		
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
