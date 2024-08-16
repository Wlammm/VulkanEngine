#include "EnginePch.h"
#include "ShadowPipeline.h"
#include "Engine.h"
#include "Assets/AssetRegistry.h"
#include "Vulkan/VulkanShader.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vertex.hpp"
#include "Components/DirectionalLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TransformComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "World/World.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanImage.h"

ShadowPipeline::ShadowPipeline()
{
	myVertexShader = Engine::GetAssetRegistry().GetShader("VertexShader.vert");
	myVertexShader->AddObserver(this);

	myFrameDataBuffer = VulkanAllocator::AllocateBuffer_TS(
		"FrameDataBuffer",
		VulkanBuffer::UniformBufferCreateInfo(sizeof(FrameData)),
		VMA_MEMORY_USAGE_AUTO,
		true);

	CreateRenderPass();
	CreateDescriptors();
	CreatePipeline();

	VulkanImage* directionalLightShadowMap = Engine::GetEngineSystem<RenderSystem>().GetDirectionalLightShadowMap();
	std::array<vk::ImageView, 1> attachments = { directionalLightShadowMap->GetImageView() };
	myDirectionalLightFrameBuffer = VulkanContext::GetDevice()->createFramebuffer(vk::FramebufferCreateInfo()
								.setRenderPass(myRenderPass)
								.setAttachments(attachments)
								.setWidth(static_cast<uint>(directionalLightShadowMap->GetSize().x))
								.setHeight(static_cast<uint>(directionalLightShadowMap->GetSize().y))
								.setLayers(1));

	LOG_WARNING("ShadowPipeline uses entities without being a system. This will cause errors in the future");
}

ShadowPipeline::~ShadowPipeline()
{
	if (myDirectionalLightFrameBuffer)
		VulkanContext::GetDevice()->destroyFramebuffer(myDirectionalLightFrameBuffer);
	
	VulkanAllocator::DestroyBuffer_TS(myFrameDataBuffer);

	myVertexShader->RemoveObserver(this);

	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);
	VulkanContext::GetDevice()->destroyRenderPass(myRenderPass);
}

void ShadowPipeline::AddCommands(const vk::CommandBuffer inCommandBuffer)
{
	DirectionalLightComponent* directionalLight = Engine::GetWorld().GetDirectionalLight();
	if(!directionalLight)
		return;
	
	if (Engine::GetWorld().GetComponentSystem().GetAllGameObjectsWithComponent<StaticMeshComponent>().IsEmpty())
		return;
	
	VertexBufferSystem& vertexBufferSystem = Engine::GetEngineSystem<VertexBufferSystem>();
	inCommandBuffer.bindVertexBuffers(0, vertexBufferSystem.GetGlobalVertexBuffer()->GetAPIResource(), {0});
	
	IndexBufferSystem& indexBufferSystem = Engine::GetEngineSystem<IndexBufferSystem>();
	inCommandBuffer.bindIndexBuffer(indexBufferSystem.GetGlobalIndexBuffer()->GetAPIResource(), 0, vk::IndexType::eUint32);
	
	const vk::ClearValue clearValue = vk::ClearDepthStencilValue(1.0f, 0u);

	if (!directionalLight->IsShadowsEnabled())
		return;

	VulkanImage* directionalLightShadowMap = Engine::GetEngineSystem<RenderSystem>().GetDirectionalLightShadowMap();
	check(directionalLightShadowMap);
	
	inCommandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
									.setRenderPass(myRenderPass)
									.setFramebuffer(myDirectionalLightFrameBuffer)
									.setPClearValues(&clearValue)
									.setClearValueCount(1)
									.setRenderArea(vk::Rect2D(vk::Offset2D{}, vk::Extent2D(static_cast<uint>(directionalLightShadowMap->GetSize().x), static_cast<uint>(directionalLightShadowMap->GetSize().y))))
									, vk::SubpassContents::eInline);
	
	inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);
	
	inCommandBuffer.setViewport(0, vk::Viewport()
								.setX(0)
								.setY(0)
								.setWidth(static_cast<float>(directionalLightShadowMap->GetSize().x))
								.setHeight(static_cast<float>(directionalLightShadowMap->GetSize().y))
								.setMinDepth(0.0f)
								.setMaxDepth(1.0f));
	
	inCommandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D{}, vk::Extent2D(static_cast<uint>(directionalLightShadowMap->GetSize().x), static_cast<uint>(directionalLightShadowMap->GetSize().y))));

	TransformComponent* lightTransform = directionalLight->GetGameObject()->GetTransform();
	
	BuildFrameBuffer(lightTransform, directionalLight);
	inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 0, myFrameDescriptorSet.GetSet(), {});

	for(GameObject* object : Engine::GetWorld().GetComponentSystem().GetAllGameObjectsWithComponent<StaticMeshComponent>())
	{
		StaticMeshComponent* staticMesh = object->GetComponent<StaticMeshComponent>();
		if (!staticMesh->GetModel())
			continue;
	
		for (const Mesh& mesh : staticMesh->GetModel()->GetMeshes())
		{
			if (!mesh.myMaterial)
				continue;

			// Insert push constant with material indices here.
			PushConstantData constantData;
			constantData.myToWorld = staticMesh->GetTransform().GetMatrix();
			inCommandBuffer.pushConstants(myPipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(PushConstantData), &constantData);
			
			const VertexBufferData& vertexBufferData = vertexBufferSystem.GetVertexBufferData(mesh.VertexBuffer);
			const IndexBufferData& indexBufferData = indexBufferSystem.GetIndexBufferData(mesh.IndexBuffer);
			inCommandBuffer.drawIndexed(mesh.NumIndices, 1, indexBufferData.myOffset, vertexBufferData.myOffset, 0);
		}
	}
	
	//vk::ImageMemoryBarrier barrier = vk::ImageMemoryBarrier()
	//	.setOldLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
	//	.setNewLayout(vk::ImageLayout::eReadOnlyOptimal)
	//	.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
	//	.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
	//	.setImage(light.myShadowMap->GetAPIResource())
	//	.setSubresourceRange(vk::ImageSubresourceRange()
	//						 .setAspectMask(vk::ImageAspectFlagBits::eDepth)
	//						 .setBaseMipLevel(0)
	//						 .setLevelCount(1)
	//						 .setBaseArrayLayer(0)
	//						 .setLayerCount(1))
	//	.setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
	//	.setDstAccessMask(vk::AccessFlagBits::eShaderRead);
	//
	//inCommandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eLateFragmentTests, vk::PipelineStageFlagBits::eFragmentShader, {}, {}, {}, barrier);
	
	inCommandBuffer.endRenderPass();
}

vk::RenderPass ShadowPipeline::GetRenderPass() const
{
	return myRenderPass;
}

void ShadowPipeline::OnAssetUpdated()
{
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);

	CreatePipeline();
}

void ShadowPipeline::CreateDescriptors()
{
	myFrameDescriptorSet.BindBuffer(myFrameDataBuffer, vk::ShaderStageFlagBits::eVertex, 0, vk::DescriptorType::eUniformBuffer);
	myFrameDescriptorSet.Build();
}

void ShadowPipeline::CreateRenderPass()
{
	const std::array<vk::AttachmentDescription, 1> attachments = {
		vk::AttachmentDescription()
			.setFormat(myShadowMapFormat)
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eStore)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::eDepthStencilReadOnlyOptimal),
	};// VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL
	const auto depthReference = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

	const auto subpass = vk::SubpassDescription()
		.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
		.setPDepthStencilAttachment(&depthReference);

	vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	const std::array<vk::SubpassDependency, 1> dependencies = {
		vk::SubpassDependency() 
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eEarlyFragmentTests)
			.setDstStageMask(vk::PipelineStageFlagBits::eLateFragmentTests)
			.setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
			.setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentRead)
			.setDependencyFlags(vk::DependencyFlags()),
	};

	myRenderPass = VulkanContext::GetDevice()->createRenderPass(vk::RenderPassCreateInfo().setAttachments(attachments).setSubpasses(subpass).setDependencies(dependencies));
}

void ShadowPipeline::CreatePipeline()
{
	List<vk::DescriptorSetLayout> layouts{ myFrameDescriptorSet.GetLayout() };
	const List<vk::PushConstantRange> pushConstants { vk::PushConstantRange().setSize(sizeof(PushConstantData)).setStageFlags(vk::ShaderStageFlagBits::eVertex) };
	myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts).setPushConstantRanges(pushConstants));

	const std::array<vk::PipelineShaderStageCreateInfo, 1> shaderStageInfo = {
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eVertex).setModule(*myVertexShader).setPName("main"),
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
		.setRenderPass(myRenderPass));

	check(returnValue.result == vk::Result::eSuccess);
	myPipeline = returnValue.value;
}

void ShadowPipeline::BuildFrameBuffer(const TransformComponent* inLightTransform, const DirectionalLightComponent* inLight)
{
	FrameData data = {};

	data.myProjection = inLight->GetLightProjection();
	data.myToView = glm::affineInverse(inLight->GetLightView());

	// This shouldnt be needed for this pass. It should only be used in pixel shader for lighting. (I think)
	data.myCameraPosition = glm::vec3(0, 0, 0);
	myFrameDataBuffer->SetData(data);
}

