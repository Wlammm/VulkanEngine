#include "EnginePch.h"
#include "VulkanPipeline.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanUniformBuffer.hpp"
#include "VulkanStorageBuffer.hpp"
#include "VulkanShader.h"
#include "VulkanImage.h"

#include "Rendering/Vertex.hpp"
#include "Containers/List.hpp"

VulkanPipeline::VulkanPipeline(const CreateInfo& inCreateInfo)
	: myCreateInfo{ inCreateInfo }
{
	CreateDescriptorPool(inCreateInfo);
	CreateDescriptors(inCreateInfo);
	CreatePipeline(inCreateInfo);
}

VulkanPipeline::~VulkanPipeline()
{
	VulkanContext::GetDevice()->waitIdle();

	VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
	VulkanContext::GetDevice()->destroyPipeline(myPipeline);
	VulkanContext::GetDevice()->destroyDescriptorSetLayout(myDescLayout);
	VulkanContext::GetDevice()->destroyDescriptorPool(myDescriptorPool);
}

const vk::Pipeline& VulkanPipeline::GetPipeline() const
{
	return myPipeline;
}

const vk::PipelineLayout& VulkanPipeline::GetPipelineLayout() const
{
	return myPipelineLayout;
}

const vk::DescriptorSet& VulkanPipeline::GetDescriptorSet() const
{
	return myDescriptorSets[VulkanContext::GetSwapChain().GetSwapChainIndex()];
}

void VulkanPipeline::CreatePipeline(const CreateInfo& inCreateInfo)
{
	const auto pPipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo().setSetLayouts(myDescLayout);

	vk::Result result = VulkanContext::GetDevice()->createPipelineLayout(&pPipelineLayoutCreateInfo, nullptr, &myPipelineLayout);
	check(result == vk::Result::eSuccess);

	const vk::ShaderModule vertexShader = *inCreateInfo.VertexShader;
	const vk::ShaderModule fragmentShader = *inCreateInfo.FragmentShader;

	const std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageInfo = {
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eVertex).setModule(vertexShader).setPName("main"),
		vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eFragment).setModule(fragmentShader).setPName("main"),
	};

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo = vk::PipelineVertexInputStateCreateInfo().setVertexAttributeDescriptions(Vertex::GetAttributeDescriptions()).setVertexBindingDescriptions(Vertex::GetBindingDescriptions());

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
}

void VulkanPipeline::CreateDescriptorPool(const CreateInfo& inCreateInfo)
{
	List<vk::DescriptorPoolSize> poolSizes;
	for (const auto& buffer : inCreateInfo.UniformBuffers)
	{
		poolSizes.Add(vk::DescriptorPoolSize()
			.setType(vk::DescriptorType::eUniformBuffer)
			.setDescriptorCount(static_cast<uint32_t>(VulkanContext::GetSwapChain().GetFrameLag()))
		);
	}

	const auto createInfo = vk::DescriptorPoolCreateInfo().setMaxSets(VulkanContext::GetSwapChain().GetFrameLag()).setPoolSizes(poolSizes);
	myDescriptorPool = VulkanContext::GetDevice()->createDescriptorPool(createInfo);
}

void VulkanPipeline::CreateDescriptors(const CreateInfo& inCreateInfo)
{
	List<vk::DescriptorSetLayoutBinding> layoutBindings;
	for (const auto& buffer : inCreateInfo.UniformBuffers)
	{
		layoutBindings.Add(vk::DescriptorSetLayoutBinding()
			.setBinding(buffer->GetBindingIndex())
			.setDescriptorType(vk::DescriptorType::eUniformBuffer)
			.setDescriptorCount(1)
			.setStageFlags(buffer->GetShaderStageFlags())
			.setPImmutableSamplers(nullptr));
	};

	for (const auto& buffer : inCreateInfo.StorageBuffers)
	{
		layoutBindings.Add(vk::DescriptorSetLayoutBinding()
						   .setBinding(buffer->GetBindingIndex())
						   .setDescriptorType(vk::DescriptorType::eStorageBufferDynamic)
						   .setDescriptorCount(1)
						   .setStageFlags(buffer->GetShaderStageFlags())
						   .setPImmutableSamplers(nullptr));
	};

	for(const auto& texture : inCreateInfo.Images)
	{
		// hard coded in binding for now. Need better solution.
		layoutBindings.Add(vk::DescriptorSetLayoutBinding()
			.setBinding(2)
			.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
			.setDescriptorCount(1)
			.setStageFlags(vk::ShaderStageFlagBits::eFragment)
			.setPImmutableSamplers(nullptr));
	}

	const auto descriptor_layout = vk::DescriptorSetLayoutCreateInfo().setBindings(layoutBindings);

	auto result = VulkanContext::GetDevice()->createDescriptorSetLayout(&descriptor_layout, nullptr, &myDescLayout);
	check(result == vk::Result::eSuccess);

	const auto allocInfo = vk::DescriptorSetAllocateInfo().setDescriptorPool(myDescriptorPool).setSetLayouts(myDescLayout);
	
	for (uint i = 0; i < VulkanContext::GetSwapChain().GetFrameLag(); ++i)
	{
		myDescriptorSets.Add(VulkanContext::GetDevice()->allocateDescriptorSets(allocInfo).front());

		List<vk::WriteDescriptorSet> writes;

		List<vk::DescriptorBufferInfo, uint> bufferInfos;
		if (!inCreateInfo.UniformBuffers.IsEmpty())
		{
			writes.Emplace();
			for (const auto& buffer : inCreateInfo.UniformBuffers)
			{
				bufferInfos.Add(vk::DescriptorBufferInfo().setOffset(0).setRange(buffer->GetBufferSize()).setBuffer(buffer->GetBuffer(i)));
			}
			writes.Last()
				.setDescriptorCount(bufferInfos.size())
				.setDescriptorType(vk::DescriptorType::eUniformBuffer)
				.setPBufferInfo(bufferInfos.data());
			writes.Last().setDstSet(myDescriptorSets[i]);
		}

		List<vk::DescriptorImageInfo, uint> textureInfos;
		if (!inCreateInfo.Images.IsEmpty())
		{
			writes.Emplace();
			for (const VulkanImage* image : inCreateInfo.Images)
			{
				textureInfos.Add(vk::DescriptorImageInfo()
					.setSampler(VulkanUtils::GetSampler(SamplerMode::Wrap))
					.setImageView(image->GetImageView())
					.setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal));
			}
			writes.Last()
				.setDstBinding(2)
				.setDescriptorCount(textureInfos.size())
				.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
				.setImageInfo(textureInfos);
			writes.Last().setDstSet(myDescriptorSets[i]);
		}

		VulkanContext::GetDevice()->updateDescriptorSets(writes, {});
	}

	/*for (const auto& buffer : inCreateInfo.UniformBuffers)
	{
		List< vk::DescriptorBufferInfo> bufferInfos;
		for(int i = 0 ; i < VulkanContext::GetSwapChain().GetFrameLag(); ++i)
		{
			auto bufferInfo = vk::DescriptorBufferInfo().setOffset(0).setRange(buffer->GetBufferSize());
			bufferInfo.setBuffer(buffer->GetBuffer(i));
			bufferInfos.Add(bufferInfo);
		}

		std::array<vk::WriteDescriptorSet, 1> writes;
		writes[0].setDescriptorCount(bufferInfos.size()).setDescriptorType(vk::DescriptorType::eUniformBuffer).setPBufferInfo(bufferInfos.data());
		writes[0].setDstSet(myDescriptorSets[]);
		VulkanContext::GetDevice()->updateDescriptorSets(writes, {});
	}*/
}
