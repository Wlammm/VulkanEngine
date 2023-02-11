#pragma once 


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

private:
	vk::ShaderModule CreateShaderFromFile(const std::string& inPath);

	void CreateDescriptorsAndPipelineLayout();
	void CreatePipeline(const CreateInfo& inCreateInfo);

private:
	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	vk::DescriptorSetLayout myDescLayout;
};