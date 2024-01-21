#pragma once

class VulkanDescriptorSet
{
public:
	VulkanDescriptorSet();
	VulkanDescriptorSet(vk::DescriptorSetLayout inLayout);
	~VulkanDescriptorSet();

	vk::DescriptorSetLayout GetLayout() const;
	vk::DescriptorSet GetSet() const;

	// Add all bindings first, then call Build().
	void BindUniformBuffer(const class IVulkanUniformBuffer& inUniformBuffer);
	void BindStorageBuffer(const class IVulkanStorageBuffer& inStorageBuffer);
	void BindBuffer(const class VulkanBuffer* inBuffer, vk::ShaderStageFlags inShaderStages, uint inBindingIndex, vk::DescriptorType inDescriptorType);
	void BindImage(const class VulkanImage* inImage, const vk::Sampler inSampler, const uint inBinding, const vk::ShaderStageFlags inShaderFlags);

	void Build();

private:
	// If a layout is passed in as constructor we do not want to handle deletion of it.
	bool myUsesSharedLayout = false;
	vk::DescriptorSetLayout myLayout;
	vk::DescriptorSet mySet;

	List<vk::DescriptorSetLayoutBinding> myLayoutBindings;
	List<vk::WriteDescriptorSet> mySetWrites;

	// These are linked list so references arent broken on resize.
	std::list<vk::DescriptorBufferInfo> myBufferInfos;
	std::list<vk::DescriptorImageInfo> myImageInfos;
};