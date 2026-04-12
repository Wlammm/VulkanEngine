#include "EnginePch.h"
#include "RenderSystem.h"
#if DEBUG
#include <stacktrace>
#endif

#include "GPUResourceManager.h"
#include "Engine/Engine.h"
#include "IndexBufferSystem.h"
#include "TLAS.h"
#include "VertexBufferSystem.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/DirectionalLightComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Systems/PointLightSystem.h"
#include "Engine/Vulkan/GPUSceneSystem.h"
#include "Engine/Vulkan/VulkanCommandBuffer.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanImage.h"
#include "Engine/Vulkan/VulkanPhysicalDevice.h"
#include "Engine/Vulkan/VulkanSwapChain.h"
#include "Engine/Vulkan/Containers/ConstantBuffer.h"
#include "Engine/World/World.h"
#include "RenderGraph/RenderGraph.h"
#include "RenderingPasses/IRenderPass.h"
#include "RenderingPasses/ComputePasses/IndirectCullPass.h"
#include "RenderingPasses/ComputePasses/IndirectPrePass.h"
#include "RenderingPasses/GraphicsPasses/CopyToSwapchainPass.h"
#include "RenderingPasses/GraphicsPasses/DebugPass.h"
#include "RenderingPasses/GraphicsPasses/ImGuiPass.h"
#include "RenderingPasses/GraphicsPasses/MainPass.h"
#include "RenderingPasses/GraphicsPasses/NoDepthPass.h"
#include "RenderingPasses/GraphicsPasses/PrePass.h"
#include "RenderingPasses/GraphicsPasses/PresentPass.h"
#include "RenderingPasses/GraphicsPasses/SkyboxPass.h"

class GPUSceneSystem;

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

	DestroyRenderPasses();
	
	// Flush out remaining upload commands as there might still be buffers waiting destruction in them.
	FlushUploadCommands();

	LOG_WARNING("RenderSubsystem::~RenderSubsystem waits for gpu idle.");
	VulkanContext::GetDevice()->waitIdle();

	del(myTLAS);
	DestroyRenderResources();
}

void RenderSystem::Init()
{
	myTLAS = new TLAS();
	RegisterRenderResources();
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
		ExecuteRenderGraph(commandBuffer);
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

class VulkanImage* RenderSystem::GetResolvedDepthTexture() const
{
	return myResolvedDepthTexture;
}

void RenderSystem::OnSwapChainResize()
{
	FlushUploadCommands();
	VulkanContext::GetDevice()->waitIdle();
	DestroyRenderPasses();
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

	List<ResourceUsage> resourceUsages{};
	resourceUsages.Emplace().SetToImage(myResolvedDepthTexture, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferRead, vk::ImageLayout::eTransferSrcOptimal);

	myRenderGraph->InsertResourceBarriers(commandBuffer->GetAPIResource(), resourceUsages);

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
	std::string debugName = "Upload command buffer";
	if (myCaptureUploadCmdBufferCallstacks)
	{
		debugName += '\n';
		debugName += std::to_string(std::stacktrace::current(1));
	}
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
														   .setObjectHandle(VulkanContext::GetVulkanHandle(commandBuffer->GetAPIResource()))
														   .setPObjectName(debugName.c_str())
														   .setObjectType(vk::ObjectType::eCommandBuffer));
#endif

	return commandBuffer;
}

void RenderSystem::QueueCommandBufferForUpload_TS(VulkanCommandBuffer* inCommandBuffer, const List<ResourceUsage>& inResourceUsages)
{
	std::scoped_lock lock(myUploadMutex);
	UploadCommand& uploadCommand = myQueuedUploadCommandBuffers.Emplace();
	uploadCommand.myCommandBuffer = inCommandBuffer;
	uploadCommand.myResourceUsage = inResourceUsages;
}

TLAS* RenderSystem::GetTLAS()
{
	return myInstance->myTLAS;
}

void RenderSystem::ExecuteRenderGraph(vk::CommandBuffer inCommandBuffer)
{
	ZoneScoped;

	// If you've done buffer copies for vertex/index buffers before this:
	vk::BufferMemoryBarrier vertexBufferBarrier{};
	vertexBufferBarrier
		.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite) // Buffer copy writes
		.setDstAccessMask(vk::AccessFlagBits::eVertexAttributeRead) // Vertex shader reads
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setBuffer(GPUResourceManager::Get()->GetBuffer<Vertex>()->GetBuffer()->GetAPIResource())
		.setOffset(0)
		.setSize(VK_WHOLE_SIZE);

	vk::BufferMemoryBarrier indexBufferBarrier{};
	indexBufferBarrier
		.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite) // Buffer copy writes
		.setDstAccessMask(vk::AccessFlagBits::eIndexRead) // Vertex shader reads index buffer
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setBuffer(GPUResourceManager::Get()->GetBuffer<Index>()->GetBuffer()->GetAPIResource())
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
		.setDstAccessMask(vk::AccessFlagBits::eShaderRead) // Fragment shader reads point light buffer
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setBuffer(GPUResourceManager::Get()->GetBuffer<PointLightData>()->GetBuffer()->GetAPIResource())
		.setOffset(0)
		.setSize(VK_WHOLE_SIZE);

	inCommandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
		vk::PipelineStageFlagBits::eFragmentShader,
		{},
		nullptr,
		pointLightBarrier,
		nullptr);

	myRenderGraph->Execute(inCommandBuffer);
}

void RenderSystem::AddUploadPass(vk::CommandBuffer inCommandBuffer)
{
	ZoneScoped;
	GPUMARK_SCOPE(inCommandBuffer, "UploadPass");

	std::scoped_lock lock(myUploadMutex);
	for(const UploadCommand& uploadCommand : myQueuedUploadCommandBuffers)
	{
		myRenderGraph->InsertResourceBarriers(inCommandBuffer, uploadCommand.myResourceUsage);
		uploadCommand.myCommandBuffer->GetAPIResource().end();
		inCommandBuffer.executeCommands(uploadCommand.myCommandBuffer->GetAPIResource());
		VulkanAllocator::QueueDestroyCommand([commandBuffer = uploadCommand.myCommandBuffer]()
		{
			delete commandBuffer;
		});
	}
	myQueuedUploadCommandBuffers.Clear();
}

void RenderSystem::FlushUploadCommands()
{
	std::scoped_lock lock(myUploadMutex);
	VulkanCommandBuffer* commandBuffer = VulkanContext::GetDevice().CreateCommandBuffer(true);
	for (UploadCommand& uploadCommand : myQueuedUploadCommandBuffers)
	{
		myRenderGraph->InsertResourceBarriers(commandBuffer->GetAPIResource(), uploadCommand.myResourceUsage);
		uploadCommand.myCommandBuffer->GetAPIResource().end();
		commandBuffer->GetAPIResource().executeCommands(uploadCommand.myCommandBuffer->GetAPIResource());
	}

	VulkanContext::GetDevice().FlushCommandBuffer(commandBuffer);

	for (UploadCommand& uploadCommand : myQueuedUploadCommandBuffers)
	{
		del(uploadCommand.myCommandBuffer);
	}
	myQueuedUploadCommandBuffers.Clear();
}

void RenderSystem::RegisterRenderResources()
{
	GPUResourceManager* resourceManager = GPUResourceManager::Get();
	
	// ---------- ConstantBuffers ----------
	resourceManager->RegisterBuffer<CameraBuffer>(new ConstantBuffer<CameraBuffer>("CameraBuffer"), [](GPUResourceManager::BufferResource& inResource)
	{
		CameraComponent* camera = Engine::GetWorld()->GetMainCamera();
		if(!camera)
		{
			LOG_ERROR("No main camera set!");
			return;
		}
			
		CameraBuffer data{};
		data.myProjection = camera->GetProjection();
		data.myToView = glm::affineInverse(camera->GetTransform().GetMatrix());
		data.myCameraPosition = camera->GetTransform().GetPosition();
		
		static_cast<ConstantBuffer<CameraBuffer>*>(inResource.myBuffer)->SetData(data);
	});
	
	resourceManager->RegisterBuffer<SceneHeader>(new ConstantBuffer<SceneHeader>("SceneHeader"), [](GPUResourceManager::BufferResource& inResource)
	{
		SceneHeader data{};
		data.myNumMeshInstances = Engine::GetEngineSystem<GPUSceneSystem>().GetNumObjects();
		data.myNumPointLights = Engine::GetEngineSystem<PointLightSystem>().GetNumPointLights();

		static_cast<ConstantBuffer<SceneHeader>*>(inResource.myBuffer)->SetData(data);
	});
	
	resourceManager->RegisterBuffer<DirectionalLightBuffer>(new ConstantBuffer<DirectionalLightBuffer>("DirectionalLightBuffer"), [](GPUResourceManager::BufferResource& inResource)
	{
		DirectionalLightComponent* light = Engine::GetWorld()->GetDirectionalLight();
		if (!light)
		{
			LOG_WARNING("Scene does not contain a directional light.");
			return;
		}
		
		TransformComponent& transform = light->GetTransform();
		
		DirectionalLightBuffer data{};
		data.myColor = light->GetColor();
		data.myDirection = transform.GetForward();
		data.myLightView = glm::affineInverse(transform.GetMatrix());
		data.myLightProjection = light->GetLightProjection();
		
		static_cast<ConstantBuffer<DirectionalLightBuffer>*>(inResource.myBuffer)->SetData(data);
	});
	
	// ---------- StorageBuffers ----------
	resourceManager->RegisterBuffer<PerDrawData>(new ResizableBuffer(
		VulkanAllocator::AllocateBuffer_TS("PerDrawCallData Buffer",
		vk::BufferCreateInfo()
		.setSize(sizeof(vk::DrawIndexedIndirectCommand) * 16)
		.setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst),
		VMA_MEMORY_USAGE_AUTO)), 
		[](GPUResourceManager::BufferResource& inResource)
	{
		ResizableBuffer* resizableBuffer = static_cast<ResizableBuffer*>(inResource.myBuffer);
		GPUSceneSystem& objectSystem = Engine::GetEngineSystem<GPUSceneSystem>();
		
		uint drawCallRequiredSize = objectSystem.GetNumObjects() * sizeof(PerDrawData) * EShadingBin::ShadingBin_Count;
		if(drawCallRequiredSize > resizableBuffer->GetBuffer()->GetSize())
		{
			resizableBuffer->Resize(drawCallRequiredSize);
			
			// TODO: We probably shouldnt have this command buffer here.
			VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
			
			List<ResourceUsage> resourceUsages{};
			resourceUsages.Emplace().SetToBuffer(resizableBuffer, vk::PipelineStageFlagBits::eTransfer,vk::AccessFlagBits::eTransferWrite);
			
			RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer, resourceUsages);
		}
	});
	
	resourceManager->RegisterBuffer<vk::DrawIndexedIndirectCommand>(new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("IndirectDrawCommands Buffer",
		vk::BufferCreateInfo()
		.setSize(sizeof(vk::DrawIndexedIndirectCommand) * 16)
		.setUsage(vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst),
		VMA_MEMORY_USAGE_AUTO)), {"DrawIndexedIndirectCommand"}, [](GPUResourceManager::BufferResource& inResource)
	{
		ResizableBuffer* indirectCommandsBuffer = static_cast<ResizableBuffer*>(inResource.myBuffer);
		
		GPUSceneSystem& objectSystem = Engine::GetEngineSystem<GPUSceneSystem>();

		uint requiredSize = objectSystem.GetNumObjects() * sizeof(vk::DrawIndexedIndirectCommand) * EShadingBin::ShadingBin_Count;
		if(requiredSize > indirectCommandsBuffer->GetBuffer()->GetSize())
		{
			indirectCommandsBuffer->Resize(requiredSize);
			
			VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
			
			List<ResourceUsage> resourceUsages{};
			resourceUsages.Emplace().SetToBuffer(indirectCommandsBuffer, vk::PipelineStageFlagBits::eTransfer,vk::AccessFlagBits::eTransferWrite);
			
			RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer, resourceUsages);
		}
	});
	
	// ---------- AccelerationStructures ----------
	resourceManager->RegisterAccelerationStructure(myTLAS, {"TLAS"});
}

void RenderSystem::CreateRenderResources()
{
	CreateRenderTextures();
	CreateBuffers();
	CreateRenderPasses();
}

void RenderSystem::DestroyRenderResources()
{
	VulkanAllocator::DestroyImage_TS(myRenderTexture);
	VulkanAllocator::DestroyImage_TS(myResolvedRenderTexture);
	VulkanAllocator::DestroyImage_TS(myDepthBuffer);
	VulkanAllocator::DestroyImage_TS(myResolvedDepthTexture);
	del(myRenderGraph);
	VulkanAllocator::DestroyBuffer_TS(myCountBuffer);
}

void RenderSystem::CreateRenderPasses()
{
	myRenderGraph = new RenderGraph();
	
	myRenderGraph->AddPass(new SkyboxPass());
	myRenderGraph->AddPass(new IndirectPrePass());
	myRenderGraph->AddPass(new IndirectCullPass());
	
	myRenderGraph->AddPass(new PrePass());
	myRenderGraph->AddPass(new MainPass());
	myRenderGraph->AddPass(new NoDepthPass());
	myRenderGraph->AddPass(new DebugPass());
	myRenderGraph->AddPass(new CopyToSwapchainPass(RenderSystem::Get()->GetResolvedRenderTexture()));
	myRenderGraph->AddPass(new ImGuiPass());
	myRenderGraph->AddPass(new PresentPass());
}

void RenderSystem::DestroyRenderPasses()
{
	myRenderGraph->DestroyRenderPasses();
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

void RenderSystem::CreateBuffers()
{
	 const GPUSceneSystem& objectSystem = Engine::GetEngineSystem<GPUSceneSystem>();
    
    myCountBuffer = VulkanAllocator::AllocateBuffer_TS("IndirectDrawCount Buffer",
        vk::BufferCreateInfo()
        .setSize(sizeof(uint) * EShadingBin::ShadingBin_Count)
        .setUsage(vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst),
        VMA_MEMORY_USAGE_AUTO);

    // Count buffer is a plain uint array — not reflected, so register by shader variable name.
    // Shaders must name it 'outCountBuffer' or 'inCountBuffer' for auto-binding to resolve it.
    GPUResourceManager::Get()->RegisterBuffer(myCountBuffer, {"outCountBuffer", "inCountBuffer"});
    
    const uint numObjects = objectSystem.GetNumObjects() != 0 ? objectSystem.GetNumObjects() : 4;
}
