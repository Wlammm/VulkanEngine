#pragma once

class VulkanDescriptorSet
{
public:
	VulkanDescriptorSet();
	~VulkanDescriptorSet();

	vk::DescriptorSetLayout GetLayout() const;
	vk::DescriptorSet GetSet() const;

	// Add all bindings first, then call Build().
	void BindUniformBuffer(const class IVulkanUniformBuffer& inUniformBuffer);

	void Build();

private:
	vk::DescriptorSetLayout myLayout;
	vk::DescriptorSet mySet;

	List<vk::DescriptorSetLayoutBinding> myLayoutBindings;
};