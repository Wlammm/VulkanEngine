#include "EnginePch.h"
#include "VulkanDescriptorSet.h"

#include "ResizableBuffer.h"
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

	for(const BindingData<const ResizableBuffer*>& binding : myResizableBuffer)
	{
		binding.myData->OnBufferResized.UnBind(&VulkanDescriptorSet::Rebuild, this);
	}
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
	BindingData<const VulkanBuffer*> data{};
	data.myData = inBuffer;
	data.myShaderStages = inShaderStages;
	data.myBindingIndex = inBindingIndex;
	data.myDescriptorType = inDescriptorType;
	myBuffers.Add(data);
}

void VulkanDescriptorSet::BindBuffer(const ResizableBuffer* inBuffer, vk::ShaderStageFlags inShaderStages, uint inBindingIndex, vk::DescriptorType inDescriptorType)
{
	BindingData<const ResizableBuffer*> data{};
	data.myData = inBuffer;
	data.myShaderStages = inShaderStages;
	data.myBindingIndex = inBindingIndex;
	data.myDescriptorType = inDescriptorType;
	myResizableBuffer.Add(data);

	inBuffer->OnBufferResized.Bind(&VulkanDescriptorSet::Rebuild, this);
}

void VulkanDescriptorSet::BindImage(const VulkanImage* inImage, const vk::Sampler inSampler, const uint inBinding, const vk::ShaderStageFlags inShaderFlags, const vk::ImageLayout inImageLayout)
{
	BindingData<const VulkanImage*> data;
	data.myData = inImage;
	data.myShaderStages = inShaderFlags;
	data.myBindingIndex = inBinding;
	data.myDescriptorType = vk::DescriptorType::eCombinedImageSampler;
	data.myImageLayout = inImageLayout;
	data.mySampler = inSampler;
	myImages.Add(data);
}

void VulkanDescriptorSet::Build()
{
	List<vk::DescriptorSetLayoutBinding> layoutBindings{};
	List<vk::WriteDescriptorSet> setWrites{};
	
	List<vk::DescriptorBufferInfo> bufferInfos{};
	bufferInfos.Reserve(myBuffers.size() + myResizableBuffer.size());
	
	List<vk::DescriptorImageInfo> imageInfos{};
	imageInfos.Reserve(myImages.size());

	for(const BindingData<const VulkanBuffer*>& binding : myBuffers)
	{
		layoutBindings.Emplace()
		.setDescriptorCount(1)
		.setDescriptorType(binding.myDescriptorType)
		.setStageFlags(binding.myShaderStages)
		.setBinding(binding.myBindingIndex);

		bufferInfos.Emplace()
			.setOffset(0)
			.setBuffer(binding.myData->GetAPIResource())
			.setRange(binding.myData->GetSize());

		setWrites.Emplace()
			.setDescriptorType(binding.myDescriptorType)
			.setDstBinding(binding.myBindingIndex)
			.setBufferInfo(bufferInfos.Last());
	}
	
	for(const BindingData<const ResizableBuffer*>& binding : myResizableBuffer)
	{
		layoutBindings.Emplace()
		.setDescriptorCount(1)
		.setDescriptorType(binding.myDescriptorType)
		.setStageFlags(binding.myShaderStages)
		.setBinding(binding.myBindingIndex);

		bufferInfos.Emplace()
			.setOffset(0)
			.setBuffer(binding.myData->GetBuffer()->GetAPIResource())
			.setRange(binding.myData->GetBuffer()->GetSize());

		setWrites.Emplace()
			.setDescriptorType(binding.myDescriptorType)
			.setDstBinding(binding.myBindingIndex)
			.setBufferInfo(bufferInfos.Last());
	}

	for(const BindingData<const VulkanImage*>& binding : myImages)
	{
		layoutBindings.Add(vk::DescriptorSetLayoutBinding()
		.setBinding(binding.myBindingIndex)
		.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
		.setDescriptorCount(1)
		.setStageFlags(binding.myShaderStages)
		.setPImmutableSamplers(nullptr));

		imageInfos.Emplace()
			.setSampler(binding.mySampler)
			.setImageView(binding.myData->GetImageView())
			.setImageLayout(binding.myImageLayout);

		setWrites.Emplace()
			.setDstBinding(binding.myBindingIndex)
			.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
			.setImageInfo(imageInfos.Last());
	}

	// Create layout
	if(!myLayout)
	{
		vk::DescriptorSetLayoutCreateInfo createInfo = vk::DescriptorSetLayoutCreateInfo().setBindings(layoutBindings);
		myLayout = VulkanContext::GetDevice()->createDescriptorSetLayout(createInfo);
	}

	// Create set
	vk::DescriptorSetAllocateInfo allocInfo = vk::DescriptorSetAllocateInfo()
		.setDescriptorPool(VulkanContext::GetDescriptorPool())
		.setSetLayouts(myLayout);
	mySet = VulkanContext::GetDevice()->allocateDescriptorSets(allocInfo).front();

	// Update set
	for(vk::WriteDescriptorSet& write : setWrites)
	{
		write.setDstSet(mySet);
	}
	VulkanContext::GetDevice()->updateDescriptorSets(setWrites, {});
}

void VulkanDescriptorSet::Rebuild()
{
	if(!myUsesSharedLayout)
	{
		VulkanContext::GetDevice()->destroyDescriptorSetLayout(myLayout);
		myLayout = nullptr;
	}
	
	Build();
}
