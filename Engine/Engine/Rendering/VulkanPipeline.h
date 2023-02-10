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
	vk::ShaderModule CompileAndGetShaderFromFile(const std::string& inPath);

	vk::Pipeline myPipeline;

private:
	inline static vk::PipelineCache* myPipelineCache;
};