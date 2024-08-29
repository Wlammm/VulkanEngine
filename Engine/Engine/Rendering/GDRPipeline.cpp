#include "EnginePch.h"
#include "GDRPipeline.h"

#include "Engine.h"
#include "IndexBufferSystem.h"
#include "MeshSystem.h"
#include "TextureSystem.h"
#include "VertexBufferSystem.h"
#include "AssetRegistry/AssetRegistry.h"
#include "Assets/Shader.h"
#include "Components/CameraComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/TransformComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "ComponentSystem/GameObject.h"
#include "Shaders/MeshStructs.hpp"
#include "Systems/PointLightSystem.h"
#include "Vulkan/ObjectSystem.h"
#include "Vulkan/ResizableBuffer.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanSwapchain.h"
#include "Vulkan/VulkanUtils.hpp"
#include "World/World.h"

GDRPipeline::GDRPipeline()
{
    myCullShader = Engine::GetAssetRegistry().GetAssetSynchronous<Shader>("IndirectCulling.comp");
    myPrePassShader = Engine::GetAssetRegistry().GetAssetSynchronous<Shader>("PrePass.comp");

    CreateBuffers();
    CreatePrePassResources();
    CreateCullPassResources();
	CreateDrawPassResources();
}

GDRPipeline::~GDRPipeline()
{
    myPrePass.Destroy();
    myCullPass.Destroy();

	VulkanContext::GetDevice()->destroyPipeline(myPipeline);
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	
    VulkanAllocator::DestroyBuffer_TS(myIndirectCommandsBuffer);
    VulkanAllocator::DestroyBuffer_TS(myPerDrawDataBuffer);
    VulkanAllocator::DestroyBuffer_TS(myCountBuffer);
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
	
	ExecuteComputePass(inCommandBuffer, myCullPass);

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
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eIndexRead)
			.setBuffer(myPerDrawDataBuffer->GetBuffer()->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead)
			.setBuffer(myCountBuffer->GetAPIResource())
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
	VertexBufferSystem& vertexBufferSystem = Engine::GetEngineSystem<VertexBufferSystem>();
	IndexBufferSystem& indexBufferSystem = Engine::GetEngineSystem<IndexBufferSystem>();
	
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
    const ObjectSystem& objectSystem = Engine::GetEngineSystem<ObjectSystem>();
    
    myCountBuffer = VulkanAllocator::AllocateBuffer_TS("IndirectDrawCount Buffer",
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

    myPerDrawDataBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("PerDrawCallData Buffer",
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

void GDRPipeline::ExecuteComputePass(vk::CommandBuffer inCommandBuffer, const ComputePassResources& inComputePassResources)
{
    inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, inComputePassResources.myPipeline);
    inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, inComputePassResources.myPipelineLayout, 0, inComputePassResources.myDescriptorSet->GetSet(), {});
    inCommandBuffer.dispatch(1, 1, 1);
}

void GDRPipeline::EnsureCorrectBufferSizes(vk::CommandBuffer inCommandBuffer)
{
    ObjectSystem& objectSystem = Engine::GetEngineSystem<ObjectSystem>();

    size_t requiredSize = objectSystem.GetNumObjects() * sizeof(vk::DrawIndexedIndirectCommand);
    if(requiredSize > myIndirectCommandsBuffer->GetBuffer()->GetSize())
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

    size_t drawCallRequiredSize = objectSystem.GetNumObjects() * sizeof(PerDrawData);
    if(drawCallRequiredSize > myPerDrawDataBuffer->GetBuffer()->GetSize())
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
}

void GDRPipeline::CreatePrePassResources()
{
    // Create descriptors
    MeshSystem& meshSystem = Engine::GetEngineSystem<MeshSystem>();
    ObjectSystem& objectSystem = Engine::GetEngineSystem<ObjectSystem>();
    
    myPrePass.myDescriptorSet = new VulkanDescriptorSet();
    myPrePass.myDescriptorSet->BindBuffer(myCountBuffer, vk::ShaderStageFlagBits::eCompute, 0, vk::DescriptorType::eStorageBuffer);
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
    ObjectSystem& objectSystem = Engine::GetEngineSystem<ObjectSystem>();

    myCullPass.myDescriptorSet = new VulkanDescriptorSet();
    myCullPass.myDescriptorSet->BindBuffer(meshSystem.GetBuffer(), vk::ShaderStageFlagBits::eCompute, 0, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(objectSystem.GetBuffer(), vk::ShaderStageFlagBits::eCompute, 1, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(myIndirectCommandsBuffer, vk::ShaderStageFlagBits::eCompute, 2, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(myCountBuffer, vk::ShaderStageFlagBits::eCompute, 3, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(myPerDrawDataBuffer, vk::ShaderStageFlagBits::eCompute, 4, vk::DescriptorType::eStorageBuffer);
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
	myVertexShader = Engine::GetAssetRegistry().GetAssetSynchronous<Shader>("IndirectVertexShader.vert");
	myVertexShader->OnShaderRecompiled.Bind(&GDRPipeline::OnShaderRecompiled, this);
	myFragmentShader = Engine::GetAssetRegistry().GetAssetSynchronous<Shader>("IndirectFragmentShader.frag");
	myFragmentShader->OnShaderRecompiled.Bind(&GDRPipeline::OnShaderRecompiled, this);
	
    // Descriptor sets
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
		.setCullMode(vk::CullModeFlagBits::eBack)
		.setFrontFace(vk::FrontFace::eCounterClockwise)
		.setDepthBiasEnable(VK_FALSE)
		.setLineWidth(1.0f);

	const auto multisampleInfo = vk::PipelineMultisampleStateCreateInfo();
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
	const std::array<vk::DynamicState, 2> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
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
	for (GameObject* object : Engine::GetWorld().GetComponentSystem().GetAllGameObjectsWithComponent<CameraComponent>())
	{
		CameraComponent* camera = object->GetComponent<CameraComponent>();
		TransformComponent* transform = object->GetTransform();
		
		FrameData data{};
		data.myProjection = camera->GetProjection();
		data.myToView = glm::affineInverse(transform->GetMatrix());
		data.myCameraPosition = transform->GetPosition();
		myFrameDataBuffer->SetData(data);
		return;
	}

	LOG("No render camera found.");	
}

void GDRPipeline::BuildDirectionalLightBuffer() const
{
	DirectionalLightBuffer buffer = {};

	for (GameObject* object : Engine::GetWorld().GetComponentSystem().GetAllGameObjectsWithComponent<DirectionalLightComponent>())
	{
		DirectionalLightComponent* light = object->GetComponent<DirectionalLightComponent>();
		TransformComponent* transform = object->GetTransform();
		buffer.myColor = light->GetColor();
		buffer.myDirection = transform->GetForward();
		buffer.myLightView = glm::affineInverse(light->GetLightView());
		buffer.myLightProjection = light->GetLightProjection();
		myDirectionalLightBuffer->SetData(buffer);
		return;
	}
	buffer.myColor = glm::vec4(0, 0, 0, 0);
	LOG("No directional light found.");
}
