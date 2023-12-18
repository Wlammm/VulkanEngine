#include "EnginePch.h"
#include "MeshPipeline.h"

#include "Engine.h"
#include "Engine/Assets/AssetRegistry.h"

#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanShader.h"
#include "Vulkan/VulkanDescriptorSet.h"

#include "Vertex.hpp"
#include "World/World.h"
#include "ECS/Systems/RenderSystem.h"
#include "Assets/Material.h"

MeshPipeline::MeshPipeline()
{
	myVertexShader = Engine::GetAssetRegistry().GetShader("VertexShader.vert");
	myVertexShader->AddObserver(this);

	myFragmentShader = Engine::GetAssetRegistry().GetShader("FragmentShader.frag");
	myFragmentShader->AddObserver(this);

	CreateDescriptors();
	CreatePipeline();

 	LOG_WARNING("MeshPipeline uses entities without being a system. This will cause errors in the future");
}

MeshPipeline::~MeshPipeline()
{
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
	myFrameDescriptorSet.BindUniformBuffer(myFrameData);
	myFrameDescriptorSet.Build();

	myObjectDescriptorSet.BindUniformBuffer(myObjectData);
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
	FrameData& buffer = myFrameData.Get();
	auto view = Engine::GetWorld().GetRegistry().view<const Transform, const Camera>();

	for (auto ent : view)
	{
		buffer.myProjection = view.get<const Camera>(ent).myProjection.Transposed();
		buffer.myToView = view.get<const Transform>(ent).GetMatrix().FastInverse().Transposed();
		return;
	}

	LOG("No render camera found.");
}

void MeshPipeline::BuildObjectBuffer(const Transform& inTransform)
{
	ObjectData& buffer = myObjectData.Get();
	buffer.myToWorld = inTransform.GetMatrix();
}

void MeshPipeline::OnAssetUpdated()
{
	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);

	CreatePipeline();
}

