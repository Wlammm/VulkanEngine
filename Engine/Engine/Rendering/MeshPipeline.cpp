#include "EnginePch.h"
#include "MeshPipeline.h"

#include "Engine.h"
#include "Engine/Assets/AssetRegistry.h"

#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Assets/Material.h"
#include "Vulkan/VulkanShader.h"
#include "Vertex.hpp"
#include "ECS/Systems/RenderSystem.h"

MeshPipeline::MeshPipeline()
{
	myVertexShader = Engine::GetAssetRegistry().GetShader("VertexShader.vert");
	myFragmentShader = Engine::GetAssetRegistry().GetShader("FragmentShader.frag");

	CreateDescriptorSetLayouts();
	CreatePipeline();
	CreateDescriptorSets();
}

MeshPipeline::~MeshPipeline()
{
	VulkanContext::GetDevice()->destroyDescriptorSetLayout(myFrameDescriptorSetLayout);
	VulkanContext::GetDevice()->destroyDescriptorSetLayout(myObjectDescriptorSetLayout);

}

void MeshPipeline::AddDrawCommands(const vk::CommandBuffer inCommandBuffer)
{
	inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);
	//inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 0, );
}

void MeshPipeline::CreateDescriptorSetLayouts()
{
	{
		// Frame set. (Holds view & projection matrix)
		List<vk::DescriptorSetLayoutBinding> layoutBindings;
		layoutBindings.Emplace()
			.setBinding(myFrameData.GetBindingIndex())
			.setDescriptorType(vk::DescriptorType::eUniformBuffer)
			.setDescriptorCount(1)
			.setStageFlags(myFrameData.GetShaderStageFlags());

		vk::DescriptorSetLayoutCreateInfo createInfo = vk::DescriptorSetLayoutCreateInfo().setBindings(layoutBindings);
		myFrameDescriptorSetLayout = VulkanContext::GetDevice()->createDescriptorSetLayout(createInfo);
	}

	{
		// Object set.
		List<vk::DescriptorSetLayoutBinding> layoutBindings;
		layoutBindings.Emplace()
			.setBinding(myObjectData.GetBindingIndex())
			.setDescriptorType(vk::DescriptorType::eUniformBuffer)
			.setDescriptorCount(1)
			.setStageFlags(myObjectData.GetShaderStageFlags());

		vk::DescriptorSetLayoutCreateInfo createInfo = vk::DescriptorSetLayoutCreateInfo().setBindings(layoutBindings);
		myObjectDescriptorSetLayout = VulkanContext::GetDevice()->createDescriptorSetLayout(createInfo);
	}
}

void MeshPipeline::CreateDescriptorSets()
{
	{
		// Frame set allocate
		vk::DescriptorSetAllocateInfo allocInfo = vk::DescriptorSetAllocateInfo()
			.setDescriptorPool(VulkanContext::GetDescriptorPool())
			.setSetLayouts(myFrameDescriptorSetLayout);

		myFrameDescriptorSet = VulkanContext::GetDevice()->allocateDescriptorSets(allocInfo).front();
	}

	{
		// Object set allocate
		vk::DescriptorSetAllocateInfo allocInfo = vk::DescriptorSetAllocateInfo()
			.setDescriptorPool(VulkanContext::GetDescriptorPool())
			.setSetLayouts(myObjectDescriptorSetLayout);

		myObjectDescriptorSet = VulkanContext::GetDevice()->allocateDescriptorSets(allocInfo).front();
	}

	// Frame set update
	List<vk::WriteDescriptorSet> writes;
	writes.Emplace()
		.setDstSet(myFrameDescriptorSet)
		.setDescriptorType(vk::DescriptorType::eUniformBuffer)
		.setDstBinding(myFrameData.GetBindingIndex())
		.setBufferInfo(vk::DescriptorBufferInfo()
			.setOffset(0)
			.setBuffer(myFrameData.GetBuffer(0))
			.setRange(myFrameData.GetBufferSize()));

	// Object set update
	writes.Emplace()
		.setDstSet(myObjectDescriptorSet)
		.setDescriptorType(vk::DescriptorType::eUniformBuffer)
		.setDstBinding(myObjectData.GetBindingIndex())
		.setBufferInfo(vk::DescriptorBufferInfo()
			.setOffset(0)
			.setBuffer(myObjectData.GetBuffer(0))
			.setRange(myObjectData.GetBufferSize()));

	VulkanContext::GetDevice()->updateDescriptorSets(writes, {});
}

void MeshPipeline::CreatePipeline()
{
	List<vk::DescriptorSetLayout> layouts{ myFrameDescriptorSetLayout, Material::GetMaterialDescriptorLayout(), myObjectDescriptorSetLayout };
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
													   .setRenderPass(Engine::GetSystem<RenderSystem>()->GetRenderPass()));

	check(returnValue.result == vk::Result::eSuccess);
	myPipeline = returnValue.value;
}
