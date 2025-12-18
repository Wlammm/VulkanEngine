#include "EnginePch.h"
#include "GDRPipeline.h"

#include "Engine/Engine.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Assets/TextureCube.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/DirectionalLightComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Rendering/SharedWithShaders/MeshStructs.hpp"
#include "Engine/Systems/PointLightSystem.h"
#include "Engine/Vulkan/GPUSceneSystem.h"
#include "Engine/Vulkan/ResizableBuffer.h"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanPhysicalDevice.h"
#include "Engine/Vulkan/VulkanSwapChain.h"
#include "Engine/World/World.h"
#include "RenderingPasses/ComputePasses/IndirectCullPass.h"
#include "RenderingPasses/ComputePasses/IndirectPrePass.h"
#include "RenderingPasses/GraphicsPasses/CopyTexturePass.h"
#include "RenderingPasses/GraphicsPasses/CopyToSwapchainPass.h"
#include "RenderingPasses/GraphicsPasses/DebugPass.h"
#include "RenderingPasses/GraphicsPasses/ImGuiPass.h"
#include "RenderingPasses/GraphicsPasses/MainPass.h"
#include "RenderingPasses/GraphicsPasses/NoDepthPass.h"
#include "RenderingPasses/GraphicsPasses/SkyboxPass.h"
#include "RenderingPasses/TransitionPasses/TransitionImagePass.h"
#include "RenderingPasses/TransitionPasses/TransitionSwapchainImagePass.h"

GDRPipeline::GDRPipeline()
{
	check(!myInstance)
	myInstance= this;
	
    CreateBuffers();
	CreateRenderPasses();
}

GDRPipeline::~GDRPipeline()
{
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	
    VulkanAllocator::DestroyBuffer_TS(myIndirectCommandsBuffer);
    VulkanAllocator::DestroyBuffer_TS(myIndirectCommandsBufferNoDepth);
    VulkanAllocator::DestroyBuffer_TS(myPerDrawDataBuffer);
    VulkanAllocator::DestroyBuffer_TS(myPerDrawDataNoDepthBuffer);
    VulkanAllocator::DestroyBuffer_TS(myCountBuffer);
    VulkanAllocator::DestroyBuffer_TS(myCountNoDepthBuffer);
    VulkanAllocator::DestroyBuffer_TS(myDirectionalLightBuffer);
    VulkanAllocator::DestroyBuffer_TS(myFrameDataBuffer);
    myCountBuffer = nullptr;
	
	DestroyRenderPasses();
	myInstance = nullptr;	
}

void GDRPipeline::ExecutePasses(vk::CommandBuffer inCommandBuffer)
{
	EnsureCorrectBufferSizes(inCommandBuffer);
	BuildFrameBuffer();
	BuildDirectionalLightBuffer();
	
	for (IRenderPass* renderPass : myRenderPasses)
	{
		renderPass->Execute(inCommandBuffer);
	}
}

VulkanBuffer* GDRPipeline::GetCountBuffer() const
{
    return myCountBuffer;
}

VulkanBuffer* GDRPipeline::GetIndirectBuffer() const
{
    return myIndirectCommandsBuffer->GetBuffer();    
}

ResizableBuffer* GDRPipeline::GetPerDrawDataBuffer() const
{
    return myPerDrawDataBuffer;
}

void GDRPipeline::CreateBuffers()
{
    const GPUSceneSystem& objectSystem = Engine::GetEngineSystem<GPUSceneSystem>();
    
    myCountBuffer = VulkanAllocator::AllocateBuffer_TS("IndirectDrawCount Buffer",
        vk::BufferCreateInfo()
        .setSize(sizeof(uint))
        .setUsage(vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer),
        VMA_MEMORY_USAGE_AUTO);

	myCountNoDepthBuffer = VulkanAllocator::AllocateBuffer_TS("IndirectDrawCount NoDepth Buffer",
		vk::BufferCreateInfo()
		.setSize(sizeof(uint))
		.setUsage(vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer),
		VMA_MEMORY_USAGE_AUTO);
    
    const uint numObjects = objectSystem.GetNumObjects() != 0 ? objectSystem.GetNumObjects() : 4;
    
    myIndirectCommandsBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("IndirectDrawCommands Buffer",
        vk::BufferCreateInfo()
        .setSize(sizeof(vk::DrawIndexedIndirectCommand) * numObjects)
        .setUsage(vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst),
        VMA_MEMORY_USAGE_AUTO));

	myIndirectCommandsBufferNoDepth = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("IndirectDrawCommands Buffer No Depth",
		vk::BufferCreateInfo()
		.setSize(sizeof(vk::DrawIndexedIndirectCommand) * numObjects)
		.setUsage(vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst),
		VMA_MEMORY_USAGE_AUTO));

    myPerDrawDataBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("PerDrawCallData Buffer",
        vk::BufferCreateInfo()
        .setSize(sizeof(vk::DrawIndexedIndirectCommand) * numObjects)
        .setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst),
        VMA_MEMORY_USAGE_AUTO));

	// TODO: This should probably not have allocations for all objects. Only the meshes that shouldnt render depth.
	myPerDrawDataNoDepthBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("PerDrawCallData NoDepth Buffer",
		vk::BufferCreateInfo()
		.setSize(sizeof(vk::DrawIndexedIndirectCommand) * numObjects)
		.setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst),
		VMA_MEMORY_USAGE_AUTO));

    myFrameDataBuffer = VulkanAllocator::AllocateBuffer_TS(
        "FrameDataBuffer",
        VulkanBuffer::UniformBufferCreateInfo(sizeof(FrameData)),
        VMA_MEMORY_USAGE_AUTO, 
        true);

    myDirectionalLightBuffer = VulkanAllocator::AllocateBuffer_TS(
        "DirectionalLightBuffer", 
        VulkanBuffer::UniformBufferCreateInfo(sizeof(DirectionalLightBuffer)), 
        VMA_MEMORY_USAGE_AUTO, 
        true);
}

void GDRPipeline::EnsureCorrectBufferSizes(vk::CommandBuffer inCommandBuffer)
{
    GPUSceneSystem& objectSystem = Engine::GetEngineSystem<GPUSceneSystem>();

    uint requiredSize = objectSystem.GetNumObjects() * sizeof(vk::DrawIndexedIndirectCommand);
    if(requiredSize > myIndirectCommandsBuffer->GetBuffer()->GetSize())
    {
	    {
		    myIndirectCommandsBuffer->Resize(requiredSize);

	    	vk::BufferMemoryBarrier bufferMemoryBarrier = vk::BufferMemoryBarrier()
				.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
				.setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite)
				.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
				.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
				.setBuffer(myIndirectCommandsBuffer->GetBuffer()->GetAPIResource())
				.setOffset(0)
				.setSize(VK_WHOLE_SIZE);

	    	// Insert the pipeline barrier
	    	inCommandBuffer.pipelineBarrier(
				vk::PipelineStageFlagBits::eTransfer,
				vk::PipelineStageFlagBits::eComputeShader,
				vk::DependencyFlags(),
				nullptr,
				bufferMemoryBarrier,
				nullptr
			);
	    }

    	// No depth
    	// TODO: This should all be refactored and nicer.
	    {
	    	myIndirectCommandsBufferNoDepth->Resize(requiredSize);

	    	vk::BufferMemoryBarrier bufferMemoryBarrier = vk::BufferMemoryBarrier()
				.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
				.setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite)
				.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
				.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
				.setBuffer(myIndirectCommandsBufferNoDepth->GetBuffer()->GetAPIResource())
				.setOffset(0)
				.setSize(VK_WHOLE_SIZE);

	    	// Insert the pipeline barrier
	    	inCommandBuffer.pipelineBarrier(
				vk::PipelineStageFlagBits::eTransfer,
				vk::PipelineStageFlagBits::eComputeShader,
				vk::DependencyFlags(),
				nullptr,
				bufferMemoryBarrier,
				nullptr
			);
	    }
    }

    uint drawCallRequiredSize = objectSystem.GetNumObjects() * sizeof(PerDrawData);
    if(drawCallRequiredSize > myPerDrawDataBuffer->GetBuffer()->GetSize())
    {
	    {
		    myPerDrawDataBuffer->Resize(drawCallRequiredSize);

	    	vk::BufferMemoryBarrier bufferMemoryBarrier = vk::BufferMemoryBarrier()
				.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
				.setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite)
				.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
				.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
				.setBuffer(myPerDrawDataBuffer->GetBuffer()->GetAPIResource())
				.setOffset(0)
				.setSize(VK_WHOLE_SIZE);

	    	// Insert the pipeline barrier
	    	inCommandBuffer.pipelineBarrier(
				vk::PipelineStageFlagBits::eTransfer,
				vk::PipelineStageFlagBits::eComputeShader,
				vk::DependencyFlags(),
				nullptr,
				bufferMemoryBarrier,
				nullptr
			);
	    }

    	// No depth
	    {
	    	myPerDrawDataNoDepthBuffer->Resize(drawCallRequiredSize);

	    	vk::BufferMemoryBarrier bufferMemoryBarrier = vk::BufferMemoryBarrier()
				.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
				.setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite)
				.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
				.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
				.setBuffer(myPerDrawDataNoDepthBuffer->GetBuffer()->GetAPIResource())
				.setOffset(0)
				.setSize(VK_WHOLE_SIZE);

	    	// Insert the pipeline barrier
	    	inCommandBuffer.pipelineBarrier(
				vk::PipelineStageFlagBits::eTransfer,
				vk::PipelineStageFlagBits::eComputeShader,
				vk::DependencyFlags(),
				nullptr,
				bufferMemoryBarrier,
				nullptr
			);
	    }
    }
}

void GDRPipeline::DestroyRenderPasses()
{
	for (IRenderPass* renderPass : myRenderPasses)
	{
		renderPass->DestroyResources();
		del(renderPass);	
	}
	myRenderPasses.Clear();
}

void GDRPipeline::CreateRenderPasses()
{
	AddGraphicsPass<TransitionImagePass>(
		RenderSystem::Get()->GetResolvedRenderTexture(),
		vk::AccessFlagBits::eNone,
		vk::AccessFlagBits::eColorAttachmentWrite,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::eColorAttachmentOptimal,
		vk::PipelineStageFlagBits::eTopOfPipe,
		vk::PipelineStageFlagBits::eColorAttachmentOutput);
	
	AddGraphicsPass<TransitionImagePass>(
	RenderSystem::Get()->myRenderTexture,
		vk::AccessFlagBits::eNone,
		vk::AccessFlagBits::eColorAttachmentWrite,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::eColorAttachmentOptimal,
		vk::PipelineStageFlagBits::eTopOfPipe,
		vk::PipelineStageFlagBits::eColorAttachmentOutput);
	
	AddGraphicsPass<SkyboxPass>();
	AddGraphicsPass<IndirectPrePass>();
	AddGraphicsPass<IndirectCullPass>();
	AddGraphicsPass<MainPass>();
	AddGraphicsPass<NoDepthPass>();
	AddGraphicsPass<DebugPass>();
	
	AddGraphicsPass<TransitionImagePass>(
		RenderSystem::Get()->GetResolvedRenderTexture(),
		vk::AccessFlagBits::eColorAttachmentWrite,
		vk::AccessFlagBits::eShaderRead,
		vk::ImageLayout::eColorAttachmentOptimal,
		vk::ImageLayout::eShaderReadOnlyOptimal,
		vk::PipelineStageFlagBits::eColorAttachmentOutput,
		vk::PipelineStageFlagBits::eFragmentShader);
	
	AddGraphicsPass<TransitionSwapchainImagePass>(
		vk::AccessFlagBits::eNone,
		vk::AccessFlagBits::eColorAttachmentWrite,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::eColorAttachmentOptimal,
		vk::PipelineStageFlagBits::eTopOfPipe,
		vk::PipelineStageFlagBits::eColorAttachmentOutput);
	
	AddGraphicsPass<CopyToSwapchainPass>(RenderSystem::Get()->GetResolvedRenderTexture());
	AddGraphicsPass<ImGuiPass>();
	
	AddGraphicsPass<TransitionSwapchainImagePass>(
		vk::AccessFlagBits::eColorAttachmentWrite,
		vk::AccessFlagBits::eNone,
		vk::ImageLayout::eColorAttachmentOptimal,
		vk::ImageLayout::ePresentSrcKHR,
		vk::PipelineStageFlagBits::eColorAttachmentOutput,
		vk::PipelineStageFlagBits::eBottomOfPipe);
}

void GDRPipeline::BuildFrameBuffer() const
{
	CameraComponent* camera = Engine::GetWorld()->GetMainCamera();

	if (!camera)
	{
		LOG_ERROR("No main camera set!");
		return;
	}
	
	TransformComponent& transform = camera->GetTransform();
	
	FrameData data{};
	data.myProjection = camera->GetProjection();
	data.myToView = glm::affineInverse(transform.GetMatrix());
	data.myCameraPosition = transform.GetPosition();

	if(myCubemap)
		data.myCubemapIndex = myCubemap->GetBindlessIndex();
	
	myFrameDataBuffer->SetData(data);
}

void GDRPipeline::BuildDirectionalLightBuffer() const
{
	DirectionalLightBuffer buffer = {};

	DirectionalLightComponent* light = Engine::GetWorld()->GetDirectionalLight();

	if (!light)
	{
		LOG_WARNING("GDRPipeline::BuildDirectionalLightBuffer - No directional light.");
		return;
	}
	
	TransformComponent& transform = light->GetTransform();
	buffer.myColor = light->GetColor();
	buffer.myDirection = transform.GetForward();
	buffer.myLightView = glm::affineInverse(transform.GetMatrix());
	buffer.myLightProjection = light->GetLightProjection();
	myDirectionalLightBuffer->SetData(buffer);
}

void GDRPipeline::OnTransformMarkedDirty(TransformComponent* inTransform)
{
	myDirtyTransforms.Add(inTransform);
}
