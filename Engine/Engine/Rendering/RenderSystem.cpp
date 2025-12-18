#include "EnginePch.h"
#include "RenderSystem.h"

#include "Engine/Engine.h"
#include "GDRPipeline.h"
#include "IndexBufferSystem.h"
#include "VertexBufferSystem.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Systems/PointLightSystem.h"
#include "Engine/Vulkan/VulkanCommandBuffer.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanImage.h"
#include "Engine/Vulkan/VulkanPhysicalDevice.h"
#include "Engine/Vulkan/VulkanSwapChain.h"

RenderSystem::RenderSystem()
{
	check(!myInstance);
	myInstance = this;
	VulkanSwapChain::OnSwapChainResized.Bind(&RenderSystem::OnSwapChainResize,this);
}

RenderSystem::~RenderSystem()
{
	myInstance = nullptr;
	VulkanSwapChain::OnSwapChainResized.UnBind(&RenderSystem::OnSwapChainResize,this);
	
	// Flush out remaining upload commands as there might still be buffers waiting destruction in them.
	FlushUploadCommands();

	LOG_WARNING("RenderSubsystem::~RenderSubsystem waits for gpu idle.");
	VulkanContext::GetDevice()->waitIdle();

	DestroyRenderResources();
}

void RenderSystem::Init()
{
	CreateRenderResources();
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
	}

	commandBuffer.end();
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

	myGDRPipeline->ExecutePasses(inCommandBuffer);
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

void RenderSystem::FlushUploadCommands()
{
	std::scoped_lock lock(myUploadMutex);
	VulkanContext::GetDevice().FlushSecondaryCommandBuffers(myQueuedUploadCommandBuffers);
	myQueuedUploadCommandBuffers.Clear();
}

void RenderSystem::CreateRenderResources()
{
	CreateRenderTextures();
	CreatePipelines();
}

void RenderSystem::DestroyRenderResources()
{
	VulkanAllocator::DestroyImage_TS(myRenderTexture);
	VulkanAllocator::DestroyImage_TS(myResolvedRenderTexture);
	VulkanAllocator::DestroyImage_TS(myDepthBuffer);
	VulkanAllocator::DestroyImage_TS(myResolvedDepthTexture);
	del(myGDRPipeline);
}

void RenderSystem::CreatePipelines()
{
	myGDRPipeline = new GDRPipeline();
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