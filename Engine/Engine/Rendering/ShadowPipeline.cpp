#include "EnginePch.h"
#include "ShadowPipeline.h"
#include "Engine.h"
#include "Assets/AssetRegistry.h"
#include "Vulkan/VulkanShader.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vertex.hpp"
#include "World/World.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/StaticMesh.h"
#include "ECS/Components/DirectionalLight.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanAllocator.h"
#include "ECS/Components/Camera.h"

ShadowPipeline::ShadowPipeline()
{
	myVertexShader = Engine::GetAssetRegistry().GetShader("VertexShader.vert");
	myVertexShader->AddObserver(this);

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

	CreateRenderPass();
	CreateDescriptors();
	CreatePipeline();

	LOG_WARNING("ShadowPipeline uses entities without being a system. This will cause errors in the future");
}

ShadowPipeline::~ShadowPipeline()
{
	VulkanAllocator::DestroyBuffer_TS(myFrameDataBuffer);
	VulkanAllocator::DestroyBuffer_TS(myObjectDataBuffer);

	myVertexShader->RemoveObserver(this);

	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);
	VulkanContext::GetDevice()->destroyRenderPass(myRenderPass);
}

void ShadowPipeline::AddCommands(const vk::CommandBuffer inCommandBuffer)
{
	const auto directionalLightView = Engine::GetWorld().GetRegistry().view<const Transform, const DirectionalLight>();
	if (directionalLightView.size_hint() == 0)
		return;

	const auto meshView = Engine::GetWorld().GetRegistry().view<const Transform, const StaticMesh>();
	if (meshView.size_hint() == 0)
		return;

	const vk::ClearValue clearValue = vk::ClearDepthStencilValue(1.0f, 0u);
	for (const auto [entity, lightTransform, light] : directionalLightView.each())
	{
		if (!light.myShadowMap)
			continue;

		inCommandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
										.setRenderPass(myRenderPass)
										.setFramebuffer(light.myFrameBuffer)
										.setPClearValues(&clearValue)
										.setClearValueCount(1)
										.setRenderArea(vk::Rect2D(vk::Offset2D{}, vk::Extent2D(light.myShadowMap->GetSize().x, light.myShadowMap->GetSize().y)))
										, vk::SubpassContents::eInline);

		inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);

		inCommandBuffer.setViewport(0, vk::Viewport()
									.setX(0)
									.setY(0)
									.setWidth(static_cast<float>(light.myShadowMap->GetSize().x))
									.setHeight(static_cast<float>(light.myShadowMap->GetSize().y))
									.setMinDepth(0.0f)
									.setMaxDepth(1.0f));

		inCommandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D{}, vk::Extent2D(light.myShadowMap->GetSize().x, light.myShadowMap->GetSize().y)));

		BuildFrameBuffer(lightTransform, light);
		inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 0, myFrameDescriptorSet.GetSet(), {});

		for (const auto [entity, meshTransform, mesh] : meshView.each())
		{
			if (!mesh.myModel)
				continue;

			BuildObjectBuffer(meshTransform);
			inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 1, myObjectDescriptorSet.GetSet(), {});

			for (const Mesh& mesh : mesh.myModel->GetMeshes())
			{
				if (!mesh.myMaterial)
					continue;

				mesh.Bind(inCommandBuffer);
				inCommandBuffer.drawIndexed(mesh.NumIndices, 1, 0, 0, 0);
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

	myObjectDescriptorSet.BindBuffer(myObjectDataBuffer, vk::ShaderStageFlagBits::eVertex, 0, vk::DescriptorType::eUniformBuffer);
	myObjectDescriptorSet.Build();
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
	List<vk::DescriptorSetLayout> layouts{ myFrameDescriptorSet.GetLayout(), myObjectDescriptorSet.GetLayout() };
	myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));

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
		.setRenderPass(myRenderPass));

	check(returnValue.result == vk::Result::eSuccess);
	myPipeline = returnValue.value;

}

void ShadowPipeline::BuildFrameBuffer(const Transform& inLightTransform, const DirectionalLight& inLight)
{
	FrameData buffer = {};

	buffer.myProjection = inLight.myLightProjection;
	buffer.myToView = inLight.myLightView;

	// This shouldnt be needed for this pass. It should only be used in pixel shader for lighting. (I think)
	buffer.myCameraPosition = Vec3f(0, 0, 0);
	myFrameDataBuffer->SetData(buffer);
}

void ShadowPipeline::BuildObjectBuffer(const Transform& inTransform)
{
	ObjectData buffer = {};
	buffer.myToWorld = inTransform.GetMatrix();

	myObjectDataBuffer->SetData(buffer);
}

