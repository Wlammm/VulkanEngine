#include "EnginePch.h"
#include "VulkanDescriptorSet.h"

#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanUniformBuffer.hpp"
#include "VulkanImage.h"
#include "VulkanStorageBuffer.hpp"

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

void VulkanDescriptorSet::BindUniformBuffer(const IVulkanUniformBuffer& inUniformBuffer)
{
	myLayoutBindings.Emplace()
		.setDescriptorCount(1)
		.setDescriptorType(vk::DescriptorType::eUniformBuffer)
		.setStageFlags(inUniformBuffer.GetShaderStageFlags())
		.setBinding(inUniformBuffer.GetBindingIndex());

	myBufferInfos.emplace_back()
		.setOffset(0)
		.setBuffer(inUniformBuffer.GetBuffer(0))
		.setRange(inUniformBuffer.GetBufferSize());

	mySetWrites.Emplace()
		.setDescriptorType(vk::DescriptorType::eUniformBuffer)
		.setDstBinding(inUniformBuffer.GetBindingIndex())
		.setBufferInfo(myBufferInfos.back());
}

void VulkanDescriptorSet::BindStorageBuffer(const IVulkanStorageBuffer& inStorageBuffer)
{
	myLayoutBindings.Emplace()
		.setDescriptorCount(1)
		.setDescriptorType(vk::DescriptorType::eStorageBuffer)
		.setStageFlags(inStorageBuffer.GetShaderStageFlags())
		.setBinding(inStorageBuffer.GetBindingIndex());

	myBufferInfos.emplace_back()
		.setOffset(0)
		.setBuffer(inStorageBuffer.GetBuffer()->GetAPIResource())
		.setRange(inStorageBuffer.GetBufferSize());

	mySetWrites.Emplace()
		.setDescriptorType(vk::DescriptorType::eStorageBuffer)
		.setDstBinding(inStorageBuffer.GetBindingIndex())
		.setBufferInfo(myBufferInfos.back());
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

void VulkanDescriptorSet::BindImage(const VulkanImage* inImage, const vk::Sampler inSampler, const uint inBinding, const vk::ShaderStageFlags inShaderFlags)
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
		.setImageLayout(vk::ImageLayout::eReadOnlyOptimal);

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
