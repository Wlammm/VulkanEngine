#include "EnginePch.h"
#include "VulkanDescriptorSet.h"

#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"
#include "VulkanBuffer.h"

VulkanDescriptorSet::VulkanDescriptorSet()
{
}

VulkanDescriptorSet::VulkanDescriptorSet(vk::DescriptorSetLayout inLayout)
{
	myUsesSharedLayout = true;
	myLayout = inLayout;
}

VulkanDescriptorSet::~VulkanDescriptorSet()
{
	if(!myUsesSharedLayout)
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

void VulkanDescriptorSet::BindBuffer(const VulkanBuffer* inBuffer, vk::ShaderStageFlags inShaderStages, uint inBindingIndex, vk::DescriptorType inDescriptorType)
{
	myLayoutBindings.Emplace()
		.setDescriptorCount(1)
		.setDescriptorType(inDescriptorType)
		.setStageFlags(inShaderStages)
		.setBinding(inBindingIndex);

	myBufferInfos.emplace_back()
		.setOffset(0)
		.setBuffer(inBuffer->GetAPIResource())
		.setRange(inBuffer->GetSize());

	mySetWrites.Emplace()
		.setDescriptorType(inDescriptorType)
		.setDstBinding(inBindingIndex)
		.setBufferInfo(myBufferInfos.back());
}

void VulkanDescriptorSet::BindImage(const VulkanImage* inImage, const vk::Sampler inSampler, const uint inBinding, const vk::ShaderStageFlags inShaderFlags, const vk::ImageLayout inImageLayout)
{
	myLayoutBindings.Add(vk::DescriptorSetLayoutBinding()
		.setBinding(inBinding)
		.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
		.setDescriptorCount(1)
		.setStageFlags(inShaderFlags)
		.setPImmutableSamplers(nullptr));

	myImageInfos.emplace_back()
		.setSampler(inSampler)
		.setImageView(inImage->GetImageView())
		.setImageLayout(inImageLayout);

	mySetWrites.Emplace()
		.setDstBinding(inBinding)
		.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
		.setImageInfo(myImageInfos.back());
}

void VulkanDescriptorSet::Build()
{
	// Create layout
	if(!myLayout)
	{
		vk::DescriptorSetLayoutCreateInfo createInfo = vk::DescriptorSetLayoutCreateInfo().setBindings(myLayoutBindings);
		myLayout = VulkanContext::GetDevice()->createDescriptorSetLayout(createInfo);
	}

	// Create set
	vk::DescriptorSetAllocateInfo allocInfo = vk::DescriptorSetAllocateInfo()
		.setDescriptorPool(VulkanContext::GetDescriptorPool())
		.setSetLayouts(myLayout);
	mySet = VulkanContext::GetDevice()->allocateDescriptorSets(allocInfo).front();

	// Update set
	for(vk::WriteDescriptorSet& writes : mySetWrites)
	{
		writes.setDstSet(mySet);
	}
	VulkanContext::GetDevice()->updateDescriptorSets(mySetWrites, {});
}
