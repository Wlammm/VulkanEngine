#pragma once 

#include <string>
#include "Containers/List.hpp"
import VulkanHpp;

class VulkanPipeline
{
public:
	struct CreateInfo
	{
		std::string VertexShaderPath = "";
		std::string FragmentShaderPath = "";

		vk::RenderPass RenderPass;
	};

	VulkanPipeline() = delete;
	VulkanPipeline(const CreateInfo& inCreateInfo);
	~VulkanPipeline();

	const vk::Pipeline& GetPipeline() const;
	const vk::PipelineLayout& GetPipelineLayout() const;
	const vk::DescriptorSet& GetDescriptorSet() const;

private:
	vk::ShaderModule CreateShaderFromFile(const std::string& inPath);

	void CreateDescriptorPool();
	void CreateDescriptors();
	void CreatePipeline(const CreateInfo& inCreateInfo);

private:
	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	vk::DescriptorPool myDescriptorPool;
	vk::DescriptorSetLayout myDescLayout;
	List<vk::DescriptorSet> myDescriptorSets;
};