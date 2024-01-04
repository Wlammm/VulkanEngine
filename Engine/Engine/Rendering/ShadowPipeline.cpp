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

ShadowPipeline::ShadowPipeline()
{
	myVertexShader = Engine::GetAssetRegistry().GetShader("VertexShader.vert");
	myVertexShader->AddObserver(this);

	CreateRenderPass();
	CreateDescriptors();
	CreatePipeline();

	LOG_WARNING("ShadowPipeline uses entities without being a system. This will cause errors in the future");
}

ShadowPipeline::~ShadowPipeline()
{
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

	//inCommandBuffer.beginRenderPass(vk::RenderPassBeginInfo()
	//								.setRenderPass(myRenderPass)
	//								.setFramebuffer(GetVkFrameBuffer())
	//								.setPClearValues(myClearValues)
	//								.setClearValueCount(2)
	//								.setRenderArea(vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())))
	//								, vk::SubpassContents::eInline);
	//
	inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);

	for (const auto [entity, transform, light] : directionalLightView.each())
	{
		BuildFrameBuffer(light);
		inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 0, myFrameDescriptorSet.GetSet(), {});

		for (const auto [entity, transform, mesh] : meshView.each())
		{
			if (!mesh.myModel)
				continue;

			BuildObjectBuffer(transform);
			inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 1, myObjectDescriptorSet.GetSet(), {});

			for (const Mesh& mesh : mesh.myModel->GetMeshes())
			{
				if (!mesh.myMaterial)
					continue;

				mesh.Bind(inCommandBuffer);
				inCommandBuffer.drawIndexed(mesh.NumIndices, 1, 0, 0, 0);
			}
		}
	}
}

void ShadowPipeline::OnAssetUpdated()
{
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);

	CreatePipeline();
}

void ShadowPipeline::CreateDescriptors()
{
	myFrameDescriptorSet.BindUniformBuffer(myFrameDataBuffer);
	myFrameDescriptorSet.Build();

	myObjectDescriptorSet.BindUniformBuffer(myObjectDataBuffer);
	myObjectDescriptorSet.Build();
}

void ShadowPipeline::CreateRenderPass()
{
	const std::array<vk::AttachmentDescription, 1> attachments = {
		vk::AttachmentDescription()
			.setFormat(myShadowMapFormat)
			.setSamples(vk::SampleCountFlagBits::e1)
			.setLoadOp(vk::AttachmentLoadOp::eClear)
			.setStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
			.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal),
	};
	const auto depthReference = vk::AttachmentReference().setAttachment(0).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

	const auto subpass = vk::SubpassDescription()
		.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
		.setPDepthStencilAttachment(&depthReference);

	vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
	const std::array<vk::SubpassDependency, 1> dependencies = {
		vk::SubpassDependency() 
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(stages)
			.setDstStageMask(stages)
			.setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
			.setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
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

void ShadowPipeline::BuildFrameBuffer(const DirectionalLight& inLight)
{
	FrameData& buffer = myFrameDataBuffer.Get();

	

	//for (auto ent : view)
	//{
	//	buffer.myProjection = view.get<const Camera>(ent).myProjection.Transposed();
	//	buffer.myToView = view.get<const Transform>(ent).GetMatrix().FastInverse().Transposed();
	//	buffer.myCameraPosition = view.get<const Transform>(ent).GetPosition();
	//	return;
	//}

	LOG("No render camera found.");

}

void ShadowPipeline::BuildObjectBuffer(const Transform& inTransform)
{
	ObjectData& buffer = myObjectDataBuffer.Get();
	buffer.myToWorld = inTransform.GetMatrix();
}

