#include "EnginePch.h"
#include "MeshPipeline.h"

#include "Engine.h"
#include "Engine/Assets/AssetRegistry.h"

#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanShader.h"
#include "Vulkan/VulkanDescriptorSet.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanAllocator.h"

#include "Vertex.hpp"
#include "World/World.h"
#include "ECS/Systems/RenderSystem.h"
#include "Assets/Material.h"
#include "ECS/Components/DirectionalLight.h"
#include "ECS/Components/StaticMesh.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/PointLight.h"

MeshPipeline::MeshPipeline()
{
	myVertexShader = Engine::GetAssetRegistry().GetShader("VertexShader.vert");
	myVertexShader->AddObserver(this);

	myFragmentShader = Engine::GetAssetRegistry().GetShader("FragmentShader.frag");
	myFragmentShader->AddObserver(this);

	myFrameDataBuffer = VulkanAllocator::AllocateBuffer_TS(
		"FrameDataBuffer",
		VulkanBuffer::UniformBufferCreateInfo(sizeof(FrameData)),
		VMA_MEMORY_USAGE_AUTO, 
		true);

	myObjectDataBuffer = VulkanAllocator::AllocateBuffer_TS(
		"ObjectDataBuffer",
		VulkanBuffer::UniformBufferCreateInfo(sizeof(ObjectData)),
		VMA_MEMORY_USAGE_AUTO,
		true);

	myDirectionalLightBuffer = VulkanAllocator::AllocateBuffer_TS(
		"DirectionalLightBuffer", 
		VulkanBuffer::UniformBufferCreateInfo(sizeof(DirectionalLightBuffer)), 
		VMA_MEMORY_USAGE_AUTO, 
		true);

	myPointLightBuffer = VulkanAllocator::AllocateBuffer_TS(
		"PointLightBuffer", 
		VulkanBuffer::StorageBufferCreateInfo(sizeof(PointLightData)), 
		VMA_MEMORY_USAGE_AUTO);

	CreateDescriptors();
	CreatePipeline();

 	LOG_WARNING("MeshPipeline uses entities without being a system. This will cause errors in the future");
}

MeshPipeline::~MeshPipeline()
{
	VulkanAllocator::DestroyBuffer_TS(myFrameDataBuffer);
	VulkanAllocator::DestroyBuffer_TS(myObjectDataBuffer);
	VulkanAllocator::DestroyBuffer_TS(myDirectionalLightBuffer);
	VulkanAllocator::DestroyBuffer_TS(myPointLightBuffer);

	myVertexShader->RemoveObserver(this);
	myFragmentShader->RemoveObserver(this);
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);
}

void MeshPipeline::AddDrawCommands(const vk::CommandBuffer inCommandBuffer)
{
	const auto view = Engine::GetWorld().GetRegistry().view<const Transform, const StaticMesh>();
	if (view.size_hint() == 0)
		return;

	inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);

	BuildFrameBuffer();
	BuildPointLightBuffer();
	BuildDirectionalLightBuffer();

	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 0, myFrameDescriptorSet.GetSet(), {});

	for (const auto [entity, transform, mesh] : view.each())
	{
		if (!mesh.myModel)
			continue;

		BuildObjectBuffer(transform);
		inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 1, myObjectDescriptorSet.GetSet(), {});

		for (const Mesh& mesh : mesh.myModel->GetMeshes())
		{
			if (!mesh.myMaterial)
				continue;

			inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 2, mesh.myMaterial->GetDescriptorSet(), {});

			mesh.Bind(inCommandBuffer);
			inCommandBuffer.drawIndexed(mesh.NumIndices, 1, 0, 0, 0);
		}
	}
}

void MeshPipeline::CreateDescriptors()
{
	myFrameDescriptorSet.BindBuffer(
		myFrameDataBuffer, 
		vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 
		0, 
		vk::DescriptorType::eUniformBuffer);

	myFrameDescriptorSet.BindBuffer(
		myPointLightBuffer, 
		vk::ShaderStageFlagBits::eFragment, 
		1, 
		vk::DescriptorType::eStorageBuffer);

	myFrameDescriptorSet.BindBuffer(
		myDirectionalLightBuffer,
		vk::ShaderStageFlagBits::eFragment, 
		2, 
		vk::DescriptorType::eUniformBuffer);

	if (!Engine::GetWorld().GetDirectionalLight().myShadowMap)
		Engine::GetWorld().GetDirectionalLight().CreateShadowMap({ 4096, 4096 });

	myFrameDescriptorSet.BindImage(
		Engine::GetWorld().GetDirectionalLight().myShadowMap, 
		VulkanUtils::GetSampler(SamplerMode::Clamp), 
		3, 
		vk::ShaderStageFlagBits::eFragment,
		vk::ImageLayout::eDepthStencilReadOnlyOptimal);
	myFrameDescriptorSet.Build();

	myObjectDescriptorSet.BindBuffer(
		myObjectDataBuffer,
		vk::ShaderStageFlagBits::eVertex, 
		0, 
		vk::DescriptorType::eUniformBuffer);
	myObjectDescriptorSet.Build();
}

void MeshPipeline::CreatePipeline()
{
	List<vk::DescriptorSetLayout> layouts{ myFrameDescriptorSet.GetLayout(), myObjectDescriptorSet.GetLayout(), Material::GetMaterialDescriptorLayout() };
	myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));

	const std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageInfo = {
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eVertex).setModule(*myVertexShader).setPName("main"),
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eFragment).setModule(*myFragmentShader).setPName("main"),
	};

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo = vk::PipelineVertexInputStateCreateInfo().setVertexAttributeDescriptions(Vertex::GetAttributeDescriptions()).setVertexBindingDescriptions(Vertex::GetBindingDescriptions());

	const vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo().setTopology(vk::PrimitiveTopology::eTriangleList);
	const vk::PipelineViewportStateCreateInfo viewportInfo = vk::PipelineViewportStateCreateInfo().setViewportCount(1).setScissorCount(1);
	const vk::PipelineRasterizationStateCreateInfo rasterizationInfo = vk::PipelineRasterizationStateCreateInfo()
		.setDepthClampEnable(VK_FALSE)
		.setRasterizerDiscardEnable(VK_FALSE)
		.setPolygonMode(vk::PolygonMode::eFill)
		.setCullMode(vk::CullModeFlagBits::eBack)
		.setFrontFace(vk::FrontFace::eClockwise)
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
													   .setRenderPass(Engine::GetSystem<RenderSystem>()->GetRenderPass()));

	check(returnValue.result == vk::Result::eSuccess);
	myPipeline = returnValue.value;
}

void MeshPipeline::BuildFrameBuffer()
{
	auto view = Engine::GetWorld().GetRegistry().view<const Transform, const Camera>();

	for (auto ent : view)
	{
		FrameData data{};
		data.myProjection = view.get<const Camera>(ent).myProjection.Transposed();
		data.myToView = view.get<const Transform>(ent).GetMatrix().FastInverse().Transposed();
		data.myCameraPosition = view.get<const Transform>(ent).GetPosition();
		myFrameDataBuffer->SetData(data);
		return;
	}

	LOG("No render camera found.");
}

void MeshPipeline::BuildPointLightBuffer()
{
	const auto view = Engine::GetWorld().GetRegistry().view<const Transform, const PointLight>();
	if (view.size_hint() == 0)
		return;

	int lightIndex = 0;
	for (const auto [entity, transform, light] : view.each())
	{
		check(lightIndex < 10 && "max pointlights atm.");

		myPointLightData.myLights[lightIndex].myPosition = transform.GetPosition();
		myPointLightData.myLights[lightIndex].myRange = light.myRange;
		myPointLightData.myLights[lightIndex].myColor = light.myColor;
		lightIndex++;
	}
	myPointLightData.myNumLights = lightIndex;
	myPointLightBuffer->SetData(myPointLightData);
}

void MeshPipeline::BuildDirectionalLightBuffer()
{
	DirectionalLightBuffer buffer = {};
	auto view = Engine::GetWorld().GetRegistry().view<const Transform, const DirectionalLight>();

	for (auto [ent, transform, light] : view.each())
	{
		buffer.myColor = light.myColor;
		buffer.myDirection = transform.GetForward();
		buffer.myLightView = light.myLightView;
		buffer.myLightProjection = light.myLightProjection;
		myDirectionalLightBuffer->SetData(buffer);
		return;
	}
	buffer.myColor = Color(0, 0, 0, 0);
	LOG("No directional light found.");
}

void MeshPipeline::BuildObjectBuffer(const Transform& inTransform)
{
	ObjectData data{};
	data.myToWorld = inTransform.GetMatrix();
	myObjectDataBuffer->SetData(data);
}

void MeshPipeline::OnAssetUpdated()
{
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);

	CreatePipeline();
}

