#include "EnginePch.h"
#include "VulkanPipeline.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"

VulkanPipeline::VulkanPipeline(const CreateInfo& inCreateInfo)
{
	const std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageInfo = {
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eVertex).setModule(CompileAndGetShaderFromFile(inCreateInfo.VertexShaderPath)).setPName("main"),
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eFragment).setModule(CompileAndGetShaderFromFile(inCreateInfo.FragmentShaderPath)).setPName("main"),
	};

	const vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

	const auto inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo().setTopology(vk::PrimitiveTopology::eTriangleList);

	const auto viewportInfo = vk::PipelineViewportStateCreateInfo().setViewportCount(1).setScissorCount(1);

	const auto rasterizationInfo = vk::PipelineRasterizationStateCreateInfo()
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

	const auto colorBlendInfo = vk::PipelineColorBlendStateCreateInfo().setAttachments(colorBlendAttachments);

	const std::array<vk::DynamicState, 2> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };

	const auto dynamicStateInfo = vk::PipelineDynamicStateCreateInfo().setDynamicStates(dynamicStates);

	auto pipeline_return = VulkanContext::GetDevice()->createGraphicsPipeline(*myPipelineCache, vk::GraphicsPipelineCreateInfo()
		.setStages(shaderStageInfo)
		.setPVertexInputState(&vertexInputInfo)
		.setPInputAssemblyState(&inputAssemblyInfo)
		.setPViewportState(&viewportInfo)
		.setPRasterizationState(&rasterizationInfo)
		.setPMultisampleState(&multisampleInfo)
		.setPDepthStencilState(&depthStencilInfo)
		.setPColorBlendState(&colorBlendInfo)
		.setPDynamicState(&dynamicStateInfo)
		.setLayout(pipeline_layout)
		.setRenderPass(inCreateInfo.RenderPass));

	THROW_IF(pipeline_return.result != vk::Result::eSuccess, "Failed to create pipeline");
	myPipeline = pipeline_return.value;

	// destroy shader modules.
}

VulkanPipeline::~VulkanPipeline()
{
}

vk::ShaderModule VulkanPipeline::CompileAndGetShaderFromFile(const std::string& inPath)
{
	return vk::ShaderModule();
}
