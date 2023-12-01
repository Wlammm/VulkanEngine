#include "EnginePch.h"
#include "VulkanDescriptorSet.h"

#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanUniformBuffer.hpp"

VulkanDescriptorSet::VulkanDescriptorSet()
{
	
}

VulkanDescriptorSet::~VulkanDescriptorSet()
{
	VulkanContext::GetDevice()->destroyDescriptorSetLayout(myLayout);
}

vk::DescriptorSetLayout VulkanDescriptorSet::GetLayout() const
{
	return myLayout;
}

vk::DescriptorSet VulkanDescriptorSet::GetSet() const
{
	return mySet;
}

void VulkanDescriptorSet::BindUniformBuffer(const IVulkanUniformBuffer& inUniformBuffer)
{
	myLayoutBindings.Emplace()
		.setDescriptorCount(1)
		.setDescriptorType(vk::DescriptorType::eUniformBuffer)
		.setStageFlags(inUniformBuffer.GetShaderStageFlags())
		.setBinding(inUniformBuffer.GetBindingIndex());
}

void VulkanDescriptorSet::Build()
{
	// Create layout
	vk::DescriptorSetLayoutCreateInfo createInfo = vk::DescriptorSetLayoutCreateInfo().setBindings(myLayoutBindings);
	myLayout = VulkanContext::GetDevice()->createDescriptorSetLayout(createInfo);

	// Create set
	vk::DescriptorSetAllocateInfo allocInfo = vk::DescriptorSetAllocateInfo()
		.setDescriptorPool(VulkanContext::GetDescriptorPool())
		.setSetLayouts(myLayout);
	mySet = VulkanContext::GetDevice()->allocateDescriptorSets(allocInfo).front();

	// Update set
	// Frame set update
	List<vk::WriteDescriptorSet> writes;
	writes.Emplace()
		.setDstSet(mySet)
		.setDescriptorType(vk::DescriptorType::eUniformBuffer)
		.setDstBinding(myFrameData.GetBindingIndex())
		.setBufferInfo(vk::DescriptorBufferInfo()
					   .setOffset(0)
					   .setBuffer(myFrameData.GetBuffer(0))
					   .setRange(myFrameData.GetBufferSize()));

	VulkanContext::GetDevice()->updateDescriptorSets(writes, {});
}
