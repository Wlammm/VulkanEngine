#pragma once 


class VulkanPipeline
{
public:
	struct CreateInfo
	{
		class VulkanShader* VertexShader = nullptr;
		class VulkanShader* FragmentShader = nullptr;

		vk::RenderPass RenderPass;

		List<class IVulkanUniformBuffer*> UniformBuffers{};
		List<class VulkanTexture*> Textures{};
	};

	VulkanPipeline() = delete;
	VulkanPipeline(const CreateInfo& inCreateInfo);
	~VulkanPipeline();

	const vk::Pipeline& GetPipeline() const;
	const vk::PipelineLayout& GetPipelineLayout() const;
	const vk::DescriptorSet& GetDescriptorSet() const;

private:
	void CreateDescriptorPool(const CreateInfo& inCreateInfo);
	void CreateDescriptors(const CreateInfo& inCreateInfo);
	void CreatePipeline(const CreateInfo& inCreateInfo);

private:
	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	vk::DescriptorPool myDescriptorPool;
	vk::DescriptorSetLayout myDescLayout;
	List<vk::DescriptorSet> myDescriptorSets;

	CreateInfo myCreateInfo;
};