#include "EnginePch.h"
#include "MeshPipeline.h"

#include "Engine.h"
#include "GDRPipeline.h"
#include "TextureSystem.h"
#include "Engine/Assets/AssetRegistry.h"

#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanShader.h"
#include "Vulkan/VulkanDescriptorSet.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanAllocator.h"

#include "Vertex.hpp"
#include "Assets/Material.h"
#include "Components/CameraComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TransformComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "Vulkan/VulkanUtils.hpp"
#include "World/World.h"

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
	VulkanAllocator::DestroyBuffer_TS(myDirectionalLightBuffer);
	VulkanAllocator::DestroyBuffer_TS(myPointLightBuffer);

	myVertexShader->RemoveObserver(this);
	myFragmentShader->RemoveObserver(this);
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);
}

void MeshPipeline::AddDrawCommands(const vk::CommandBuffer inCommandBuffer)
{
	const List<GameObject*> objectsToRender = Engine::GetWorld().GetComponentSystem().GetAllGameObjectsWithComponent<StaticMeshComponent>();
	if (objectsToRender.IsEmpty())
		return;
	
	inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);
	
	BuildFrameBuffer();
	BuildPointLightBuffer();
	BuildDirectionalLightBuffer();
	
	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 0, myFrameDescriptorSet.GetSet(), {});
	
	VertexBufferSystem& vertexBufferSystem = Engine::GetEngineSystem<VertexBufferSystem>();
	inCommandBuffer.bindVertexBuffers(0, {vertexBufferSystem.GetGlobalVertexBuffer()->GetAPIResource()}, {0});
	
	IndexBufferSystem& indexBufferSystem = Engine::GetEngineSystem<IndexBufferSystem>();
	inCommandBuffer.bindIndexBuffer(indexBufferSystem.GetGlobalIndexBuffer()->GetAPIResource(),0, vk::IndexType::eUint32);
	
	TextureSystem& textureSystem = Engine::GetEngineSystem<TextureSystem>();
	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 1, textureSystem.GetDescriptorSet(), {});

	for (GameObject* object : objectsToRender)
	{
		StaticMeshComponent* staticMesh = object->GetComponent<StaticMeshComponent>();
		TransformComponent* transform = object->GetTransform();
		if (!staticMesh->GetModel())
			continue;
	
		for (const Mesh& mesh : staticMesh->GetModel()->GetMeshes())
		{
			if (!mesh.myMaterial)
				continue;
	
			// Insert push constant with material indices here.
			PushConstantData constantData;
			constantData.myAlbedoIndex = mesh.myMaterial->GetAlbedo();
			constantData.myNormalIndex = mesh.myMaterial->GetNormal();
			constantData.myMaterialIndex = mesh.myMaterial->GetMaterial();
			constantData.myToWorld = staticMesh->GetTransform().GetMatrix();
			inCommandBuffer.pushConstants(myPipelineLayout, vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex, 0, sizeof(PushConstantData), &constantData);

			const VertexBufferData& vertexData = vertexBufferSystem.GetVertexBufferData(mesh.VertexBuffer);
			const IndexBufferData& indexData = indexBufferSystem.GetIndexBufferData(mesh.IndexBuffer);
			inCommandBuffer.drawIndexed(mesh.NumIndices, 1, indexData.myOffset, vertexData.myOffset, 0);
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

	myFrameDescriptorSet.BindImage(
		Engine::GetEngineSystem<RenderSystem>().GetDirectionalLightShadowMap(), 
		VulkanUtils::GetSampler(SamplerMode::Clamp), 
		3, 
		vk::ShaderStageFlagBits::eFragment,
		vk::ImageLayout::eDepthStencilReadOnlyOptimal);
	
	myFrameDescriptorSet.Build();
}

void MeshPipeline::CreatePipeline()
{
	TextureSystem& textureSystem = Engine::GetEngineSystem<TextureSystem>();
	
	const List<vk::DescriptorSetLayout> layouts{ myFrameDescriptorSet.GetLayout(), textureSystem.GetDescriptorLayout() };
	const List<vk::PushConstantRange> pushConstants { vk::PushConstantRange().setSize(sizeof(PushConstantData)).setStageFlags(vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex) };
	myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts).setPushConstantRanges(pushConstants));

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

void MeshPipeline::BuildFrameBuffer()
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

void MeshPipeline::BuildPointLightBuffer()
{
	List<GameObject*> pointLights = Engine::GetWorld().GetComponentSystem().GetAllGameObjectsWithComponent<PointLightComponent>();

	if(pointLights.IsEmpty())
		return;
	
	int lightIndex = 0;
	for (GameObject* object : pointLights)
	{
		PointLightComponent* light = object->GetComponent<PointLightComponent>();
		TransformComponent* transform = object->GetTransform();
		
		check(lightIndex < 10 && "max pointlights atm.");
	
		myPointLightData.myLights[lightIndex].myPosition = transform->GetPosition();
		myPointLightData.myLights[lightIndex].myRange = light->GetRange();;
		myPointLightData.myLights[lightIndex].myColor = glm::vec4(light->GetColor(), light->GetIntensity());
		lightIndex++;
	}
	myPointLightData.myNumLights = lightIndex;
	myPointLightBuffer->SetData(myPointLightData);
}

void MeshPipeline::BuildDirectionalLightBuffer()
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

void MeshPipeline::OnAssetUpdated()
{
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);

	CreatePipeline();
}

