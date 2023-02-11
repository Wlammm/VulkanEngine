#include "EnginePch.h"
#include "VulkanPipeline.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"

VulkanPipeline::VulkanPipeline(const CreateInfo& inCreateInfo)
{
	CreateDescriptorsAndPipelineLayout();
	CreatePipeline(inCreateInfo);
}

VulkanPipeline::~VulkanPipeline()
{
}

vk::ShaderModule VulkanPipeline::CreateShaderFromFile(const std::string& inPath)
{
	std::ifstream stream(inPath, std::ios::binary);
	std::string data = { std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };
	stream.close();

	return VulkanContext::GetDevice()->createShaderModule(vk::ShaderModuleCreateInfo().setCodeSize(data.size()).setPCode((uint32_t*)data.data()));
}

void VulkanPipeline::CreateDescriptorsAndPipelineLayout()
{
	std::array<vk::DescriptorSetLayoutBinding, 2> const layout_bindings = {
		vk::DescriptorSetLayoutBinding()
			.setBinding(0)
			.setDescriptorType(vk::DescriptorType::eUniformBuffer)
			.setDescriptorCount(1)
			.setStageFlags(vk::ShaderStageFlagBits::eVertex)
			.setPImmutableSamplers(nullptr),
		vk::DescriptorSetLayoutBinding()
			.setBinding(1)
			.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
			.setDescriptorCount(0)
			.setStageFlags(vk::ShaderStageFlagBits::eFragment)
			.setPImmutableSamplers(nullptr) };


	const auto descriptor_layout = vk::DescriptorSetLayoutCreateInfo().setBindings(layout_bindings);

	auto result = VulkanContext::GetDevice()->createDescriptorSetLayout(&descriptor_layout, nullptr, &myDescLayout);
	check(result == vk::Result::eSuccess);

	const auto pPipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo().setSetLayouts(myDescLayout);

	result = VulkanContext::GetDevice()->createPipelineLayout(&pPipelineLayoutCreateInfo, nullptr, &myPipelineLayout);
	check(result == vk::Result::eSuccess);
}

void VulkanPipeline::CreatePipeline(const CreateInfo& inCreateInfo)
{
	const vk::ShaderModule vertexShader = CreateShaderFromFile(inCreateInfo.VertexShaderPath);
	const vk::ShaderModule fragmentShader = CreateShaderFromFile(inCreateInfo.FragmentShaderPath);

	const std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageInfo = {
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eVertex).setModule(vertexShader).setPName("main"),
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eFragment).setModule(fragmentShader).setPName("main"),
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

	auto pipeline_return = VulkanContext::GetDevice()->createGraphicsPipeline(VulkanContext::GetPipelineCache(), vk::GraphicsPipelineCreateInfo()
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
		.setRenderPass(inCreateInfo.RenderPass));

	THROW_IF(pipeline_return.result != vk::Result::eSuccess, "Failed to create pipeline");
	myPipeline = pipeline_return.value;

	VulkanContext::GetDevice()->destroyShaderModule(vertexShader);
	VulkanContext::GetDevice()->destroyShaderModule(fragmentShader);
}
