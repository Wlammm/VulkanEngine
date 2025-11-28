#include "EnginePch.h"
#include "GDRPipeline.h"

#include "Engine/Engine.h"
#include "IndexBufferSystem.h"
#include "MeshSystem.h"
#include "TextureSystem.h"
#include "VertexBufferSystem.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/AssetRegistry/AssetRegistry2.h"
#include "Engine/Assets/Shader.h"
#include "Engine/Assets/TextureCube.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/DirectionalLightComponent.h"
#include "Engine/Components/PointLightComponent.h"
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
#include "Engine/Vulkan/VulkanSwapchain.h"
#include "Engine/Vulkan/VulkanUtils.hpp"
#include "Engine/World/World.h"

GDRPipeline::GDRPipeline()
{
    myCullShader = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Shader>("Shaders/IndirectCulling.comp");
    myPrePassShader = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Shader>("Shaders/PrePass.comp");

    CreateBuffers();
    CreatePrePassResources();
    CreateCullPassResources();
	CreateDrawPassResources();
	
	//TransformComponent::OnMarkedDirty.Bind(&GDRPipeline::OnTransformMarkedDirty, this);
}

GDRPipeline::~GDRPipeline()
{
	//TransformComponent::OnMarkedDirty.UnBind(&GDRPipeline::OnTransformMarkedDirty, this);
	
	myVertexShader->OnShaderRecompiled.UnBind(&GDRPipeline::OnShaderRecompiled, this);
	myFragmentShader->OnShaderRecompiled.UnBind(&GDRPipeline::OnShaderRecompiled, this);
	
    myPrePass.Destroy();
    myCullPass.Destroy();

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
}

void GDRPipeline::AddComputeCommands(vk::CommandBuffer inCommandBuffer)
{
	EnsureCorrectBufferSizes(inCommandBuffer);
    
	ExecuteComputePass(inCommandBuffer, myPrePass);
	
	// Insert memory barrier to ensure that the count buffer is synchronized before cull pass
	vk::BufferMemoryBarrier barrier = vk::BufferMemoryBarrier()
		.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)  // Pre-pass writes
		.setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite)   // Cull pass reads & writes
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setBuffer(myCountBuffer->GetAPIResource())
		.setOffset(0)
		.setSize(VK_WHOLE_SIZE);

	inCommandBuffer.pipelineBarrier(
			vk::PipelineStageFlagBits::eComputeShader,
			vk::PipelineStageFlagBits::eComputeShader,
			vk::DependencyFlags{},
			nullptr,
			barrier,
			nullptr);

	GPUSceneSystem& objectSystem = Engine::GetEngineSystem<GPUSceneSystem>();
	ExecuteComputePass(inCommandBuffer, myCullPass, {std::ceil(objectSystem.GetNumObjects() / 256.0f), 1, 1});

	// Memory barrier to ensure the compute shader writes are visible to subsequent draw calls
	vk::BufferMemoryBarrier bufferMemoryBarriers[] = {
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eVertexAttributeRead)
			.setBuffer(myIndirectCommandsBuffer->GetBuffer()->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eVertexAttributeRead)
			.setBuffer(myIndirectCommandsBufferNoDepth->GetBuffer()->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eIndexRead)
			.setBuffer(myPerDrawDataBuffer->GetBuffer()->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eIndexRead)
			.setBuffer(myPerDrawDataNoDepthBuffer->GetBuffer()->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead)
			.setBuffer(myCountBuffer->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead)
			.setBuffer(myCountNoDepthBuffer->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
	};

	inCommandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eComputeShader,
		vk::PipelineStageFlagBits::eDrawIndirect | vk::PipelineStageFlagBits::eVertexInput,
		vk::DependencyFlags{},
		nullptr,
		bufferMemoryBarriers,
		nullptr);
}

void GDRPipeline::AddGraphicsCommands(vk::CommandBuffer inCommandBuffer)
{
	GPUMARK_SCOPE(inCommandBuffer, "MainPass");
	VertexBufferSystem& vertexBufferSystem = Engine::GetEngineSystem<VertexBufferSystem>();
	IndexBufferSystem& indexBufferSystem = Engine::GetEngineSystem<IndexBufferSystem>();

	inCommandBuffer.setDepthWriteEnable(true);
	inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);
	BuildFrameBuffer();
	BuildDirectionalLightBuffer();
	
	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 0, myFrameDescriptorSet.GetSet(), {});
	
	inCommandBuffer.bindVertexBuffers(0, {vertexBufferSystem.GetGlobalVertexBuffer()->GetBuffer()->GetAPIResource()}, {0});
	
	inCommandBuffer.bindIndexBuffer(indexBufferSystem.GetGlobalIndexBuffer()->GetBuffer()->GetAPIResource(),0, vk::IndexType::eUint32);
	
	TextureSystem& textureSystem = Engine::GetEngineSystem<TextureSystem>();
	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 1, textureSystem.GetDescriptorSet(), {});

	inCommandBuffer.drawIndexedIndirectCount(myIndirectCommandsBuffer->GetBuffer()->GetAPIResource(), 0,
		myCountBuffer->GetAPIResource(), 0,
		static_cast<uint>(myIndirectCommandsBuffer->GetBuffer()->GetSize() / sizeof(vk::DrawIndexedIndirectCommand)),
		sizeof(vk::DrawIndexedIndirectCommand));

	inCommandBuffer.setDepthWriteEnable(false);
	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 0, myFrameNoDepthDescriptorSet.GetSet(), {});
	inCommandBuffer.drawIndexedIndirectCount(myIndirectCommandsBufferNoDepth->GetBuffer()->GetAPIResource(), 0,
			myCountNoDepthBuffer->GetAPIResource(), 0,
			static_cast<uint>(myIndirectCommandsBufferNoDepth->GetBuffer()->GetSize() / sizeof(vk::DrawIndexedIndirectCommand)),
			sizeof(vk::DrawIndexedIndirectCommand));
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

void GDRPipeline::ComputePassResources::Destroy()
{
    VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
    VulkanContext::GetDevice()->destroyPipeline(myPipeline);
    del(myDescriptorSet);
}

void GDRPipeline::ExecuteComputePass(vk::CommandBuffer inCommandBuffer, const ComputePassResources& inComputePassResources, const glm::u32vec3& inGroupCount)
{
    inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, inComputePassResources.myPipeline);
    inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, inComputePassResources.myPipelineLayout, 0, inComputePassResources.myDescriptorSet->GetSet(), {});
    inCommandBuffer.dispatch(inGroupCount.x, inGroupCount.y, inGroupCount.z);
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

void GDRPipeline::CreatePrePassResources()
{
    // Create descriptors
    MeshSystem& meshSystem = Engine::GetEngineSystem<MeshSystem>();
    GPUSceneSystem& objectSystem = Engine::GetEngineSystem<GPUSceneSystem>();
    
    myPrePass.myDescriptorSet = new VulkanDescriptorSet();
    myPrePass.myDescriptorSet->BindBuffer(myCountBuffer, vk::ShaderStageFlagBits::eCompute, 0, vk::DescriptorType::eStorageBuffer);
    myPrePass.myDescriptorSet->BindBuffer(myCountNoDepthBuffer, vk::ShaderStageFlagBits::eCompute, 1, vk::DescriptorType::eStorageBuffer);
    myPrePass.myDescriptorSet->Build();

    // Create pipeline
    const List<vk::DescriptorSetLayout> layouts{ myPrePass.myDescriptorSet->GetLayout() };
    myPrePass.myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));
    
    vk::ComputePipelineCreateInfo createInfo = vk::ComputePipelineCreateInfo();
    createInfo.setStage(vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eCompute).setModule(myPrePassShader->GetAPIResource()).setPName("main"));
    createInfo.setLayout(myPrePass.myPipelineLayout);

    const vk::ResultValue<vk::Pipeline> result = VulkanContext::GetDevice()->createComputePipeline(VulkanContext::GetPipelineCache(), createInfo);

    check(result.result == vk::Result::eSuccess);
    myPrePass.myPipeline = result.value;
}

void GDRPipeline::CreateCullPassResources()
{
    // Create descriptors
    MeshSystem& meshSystem = Engine::GetEngineSystem<MeshSystem>();
    GPUSceneSystem& objectSystem = Engine::GetEngineSystem<GPUSceneSystem>();
	VertexBufferSystem& vertexSystem = Engine::GetEngineSystem<VertexBufferSystem>();
	IndexBufferSystem& indexSystem = Engine::GetEngineSystem<IndexBufferSystem>();
	
    myCullPass.myDescriptorSet = new VulkanDescriptorSet();
    myCullPass.myDescriptorSet->BindBuffer(meshSystem.GetBuffer(), vk::ShaderStageFlagBits::eCompute, 0, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(vertexSystem.GetGlobalSparseVertexDataBuffer(), vk::ShaderStageFlagBits::eCompute, 6, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(indexSystem.GetGlobalSparseIndexDataBuffer(), vk::ShaderStageFlagBits::eCompute, 7, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(objectSystem.GetSparseBuffer(), vk::ShaderStageFlagBits::eCompute, 1, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(objectSystem.GetDenseBuffer(), vk::ShaderStageFlagBits::eCompute, 5, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(myIndirectCommandsBuffer, vk::ShaderStageFlagBits::eCompute, 2, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(myCountBuffer, vk::ShaderStageFlagBits::eCompute, 3, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(myPerDrawDataBuffer, vk::ShaderStageFlagBits::eCompute, 4, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(myPerDrawDataNoDepthBuffer, vk::ShaderStageFlagBits::eCompute, 8, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(myCountNoDepthBuffer, vk::ShaderStageFlagBits::eCompute, 9, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(myIndirectCommandsBufferNoDepth, vk::ShaderStageFlagBits::eCompute, 10, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->Build();

    // Create pipeline
    const List<vk::DescriptorSetLayout> layouts{ myCullPass.myDescriptorSet->GetLayout() };
    myCullPass.myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));
    
    vk::ComputePipelineCreateInfo createInfo = vk::ComputePipelineCreateInfo();
    createInfo.setStage(vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eCompute).setModule(myCullShader->GetAPIResource()).setPName("main"));
    createInfo.setLayout(myCullPass.myPipelineLayout);

    const vk::ResultValue<vk::Pipeline> result = VulkanContext::GetDevice()->createComputePipeline(VulkanContext::GetPipelineCache(), createInfo);

    check(result.result == vk::Result::eSuccess);
    myCullPass.myPipeline = result.value;
}

void GDRPipeline::CreateDrawPassResources()
{
	// Shaders
	myVertexShader = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Shader>("Shaders/IndirectVertexShader.vert");
	myVertexShader->OnShaderRecompiled.Bind(&GDRPipeline::OnShaderRecompiled, this);
	myFragmentShader = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Shader>("Shaders/IndirectFragmentShader.frag");
	myFragmentShader->OnShaderRecompiled.Bind(&GDRPipeline::OnShaderRecompiled, this);
	
    // Descriptor sets
	{
		myFrameDescriptorSet.BindBuffer(
			myFrameDataBuffer, 
			vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 
			0, 
			vk::DescriptorType::eUniformBuffer);
	
		myFrameDescriptorSet.BindBuffer(
			Engine::GetEngineSystem<PointLightSystem>().GetBuffer(), 
			vk::ShaderStageFlagBits::eFragment, 
			1, 
			vk::DescriptorType::eStorageBuffer);
	
		myFrameDescriptorSet.BindBuffer(
			myDirectionalLightBuffer,
			vk::ShaderStageFlagBits::eFragment, 
			2, 
			vk::DescriptorType::eUniformBuffer);

		myFrameDescriptorSet.BindImage(
			Engine::GetEngineSystem<RenderSystem>().GetDirectionalLightShadowMap(), 
			VulkanUtils::GetSampler(SamplerMode::Clamp), 
			3, 
			vk::ShaderStageFlagBits::eFragment,
			vk::ImageLayout::eDepthStencilReadOnlyOptimal);

		myFrameDescriptorSet.BindBuffer(
			myPerDrawDataBuffer,
			vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex, 
			4, 
			vk::DescriptorType::eStorageBuffer);
	
		myFrameDescriptorSet.Build();
	}

	{
		myFrameNoDepthDescriptorSet.BindBuffer(
			myFrameDataBuffer, 
			vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 
			0, 
			vk::DescriptorType::eUniformBuffer);
	
		myFrameNoDepthDescriptorSet.BindBuffer(
			Engine::GetEngineSystem<PointLightSystem>().GetBuffer(), 
			vk::ShaderStageFlagBits::eFragment, 
			1, 
			vk::DescriptorType::eStorageBuffer);
	
		myFrameNoDepthDescriptorSet.BindBuffer(
			myDirectionalLightBuffer,
			vk::ShaderStageFlagBits::eFragment, 
			2, 
			vk::DescriptorType::eUniformBuffer);

		myFrameNoDepthDescriptorSet.BindImage(
			Engine::GetEngineSystem<RenderSystem>().GetDirectionalLightShadowMap(), 
			VulkanUtils::GetSampler(SamplerMode::Clamp), 
			3, 
			vk::ShaderStageFlagBits::eFragment,
			vk::ImageLayout::eDepthStencilReadOnlyOptimal);

		myFrameNoDepthDescriptorSet.BindBuffer(
			myPerDrawDataNoDepthBuffer,
			vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex, 
			4, 
			vk::DescriptorType::eStorageBuffer);
	
		myFrameNoDepthDescriptorSet.Build();
	}

    // Pipeline creation
	CreateGraphicsPipeline();
}

void GDRPipeline::OnShaderRecompiled()
{
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);

	CreateGraphicsPipeline();
}

void GDRPipeline::CreateGraphicsPipeline()
{
	 TextureSystem& textureSystem = Engine::GetEngineSystem<TextureSystem>();
	
	const List<vk::DescriptorSetLayout> layouts{ myFrameDescriptorSet.GetLayout(), textureSystem.GetDescriptorLayout() };
	myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));

	const std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageInfo = {
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eVertex).setModule(myVertexShader->GetAPIResource()).setPName("main"),
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eFragment).setModule(myFragmentShader->GetAPIResource()).setPName("main"),
	};

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo = vk::PipelineVertexInputStateCreateInfo().setVertexAttributeDescriptions(Vertex::GetAttributeDescriptions()).setVertexBindingDescriptions(Vertex::GetBindingDescriptions());

	const vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo().setTopology(vk::PrimitiveTopology::eTriangleList);
	const vk::PipelineViewportStateCreateInfo viewportInfo = vk::PipelineViewportStateCreateInfo().setViewportCount(1).setScissorCount(1);
	const vk::PipelineRasterizationStateCreateInfo rasterizationInfo = vk::PipelineRasterizationStateCreateInfo()
		.setDepthClampEnable(VK_FALSE)
		.setRasterizerDiscardEnable(VK_FALSE)
		.setPolygonMode(vk::PolygonMode::eFill)
		.setCullMode(vk::CullModeFlagBits::eFront)
		.setFrontFace(vk::FrontFace::eCounterClockwise)
		.setDepthBiasEnable(VK_FALSE)
		.setLineWidth(1.0f);

	const auto multisampleInfo = vk::PipelineMultisampleStateCreateInfo().setRasterizationSamples(VulkanContext::GetPhysicalDevice().GetMaxMSAASamples());
	const auto stencilOp = vk::StencilOpState().setFailOp(vk::StencilOp::eKeep).setPassOp(vk::StencilOp::eKeep).setCompareOp(vk::CompareOp::eAlways);

	const auto depthStencilInfo = vk::PipelineDepthStencilStateCreateInfo()
		.setDepthTestEnable(VK_TRUE)
		.setDepthWriteEnable(VK_TRUE)
		.setDepthCompareOp(vk::CompareOp::eLessOrEqual)
		.setDepthBoundsTestEnable(VK_FALSE)
		.setStencilTestEnable(VK_FALSE)
		.setFront(stencilOp)
		.setBack(stencilOp);

	const std::array<vk::PipelineColorBlendAttachmentState, 1> colorBlendAttachments = {
		vk::PipelineColorBlendAttachmentState().setColorWriteMask(
			vk::ColorComponentFlagBits::eR |
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA) };

	const vk::PipelineColorBlendStateCreateInfo colorBlendInfo = vk::PipelineColorBlendStateCreateInfo().setAttachments(colorBlendAttachments);
	const std::array<vk::DynamicState, 3> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor, vk::DynamicState::eDepthWriteEnable };
	const vk::PipelineDynamicStateCreateInfo dynamicStateInfo = vk::PipelineDynamicStateCreateInfo().setDynamicStates(dynamicStates);

	const vk::ResultValue<vk::Pipeline> returnValue = VulkanContext::GetDevice()->createGraphicsPipeline(VulkanContext::GetPipelineCache(), vk::GraphicsPipelineCreateInfo()
													   .setStages(shaderStageInfo)
													   .setPVertexInputState(&vertexInputInfo)
													   .setPInputAssemblyState(&inputAssemblyInfo)
													   .setPViewportState(&viewportInfo)
													   .setPRasterizationState(&rasterizationInfo)
													   .setPMultisampleState(&multisampleInfo)
													   .setPDepthStencilState(&depthStencilInfo)
													   .setPColorBlendState(&colorBlendInfo)
													   .setPDynamicState(&dynamicStateInfo)
													   .setLayout(myPipelineLayout)
													   .setRenderPass(Engine::GetEngineSystem<RenderSystem>().GetRenderPass()));

	check(returnValue.result == vk::Result::eSuccess);
	myPipeline = returnValue.value;
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
