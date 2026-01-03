#include "EnginePch.h"
#include "VulkanDescriptorSet.h"

#include "ResizableBuffer.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"
#include "VulkanBuffer.h"
#include "Containers/GPUList.h"

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

	for(const BindingData<const IGPUBuffer*>& binding : myBuffers)
	{
		if (binding.myData->GetOnBufferResized())
			binding.myData->GetOnBufferResized()->UnBind(&VulkanDescriptorSet::Rebuild, this);
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


void VulkanDescriptorSet::BindBuffer(const class IGPUBuffer* inBuffer, vk::ShaderStageFlags inShaderStages,
	uint inBindingIndex, vk::DescriptorType inDescriptorType)
{
	BindingData<const IGPUBuffer*> data{};
	data.myData = inBuffer;
	data.myShaderStages = inShaderStages;
	data.myBindingIndex = inBindingIndex;
	data.myDescriptorType = inDescriptorType;
	myBuffers.Add(data);

	if(inBuffer->GetOnBufferResized() && !inBuffer->GetOnBufferResized()->IsBound(Delegate<void()>(&VulkanDescriptorSet::Rebuild, this)))
		inBuffer->GetOnBufferResized()->Bind(&VulkanDescriptorSet::Rebuild, this);
}

void VulkanDescriptorSet::BindSampler(vk::Sampler inSampler, vk::ShaderStageFlags inShaderStages, uint inBindingIndex)
{
	BindingData<vk::Sampler> data{};
	data.myData = inSampler;
	data.myBindingIndex = inBindingIndex;
	data.myShaderStages = inShaderStages;
	data.myDescriptorType = vk::DescriptorType::eSampler;
	mySamplers.Add(data);
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
	mySampledImages.Add(data);
}

void VulkanDescriptorSet::Build()
{
	ZoneScoped;
	BuildLayoutAndAllocate();
	UpdateDescriptors();
}

void VulkanDescriptorSet::Rebuild()
{
	UpdateDescriptors();
}

void VulkanDescriptorSet::BuildLayoutAndAllocate()
{
	List<vk::DescriptorSetLayoutBinding> layoutBindings{};
	
	for(const BindingData<const IGPUBuffer*>& binding : myBuffers)
	{
		layoutBindings.Emplace()
			.setDescriptorCount(1)
			.setDescriptorType(binding.myDescriptorType)
			.setStageFlags(binding.myShaderStages)
			.setBinding(binding.myBindingIndex);
	}
	
	for(const BindingData<const VulkanImage*>& binding : mySampledImages)
	{
		layoutBindings.Add(vk::DescriptorSetLayoutBinding()
			.setBinding(binding.myBindingIndex)
			.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
			.setDescriptorCount(1)
			.setStageFlags(binding.myShaderStages)
			.setPImmutableSamplers(nullptr));
	}
	
	for (const BindingData<vk::Sampler>& binding : mySamplers)
	{
		layoutBindings.Add(
			vk::DescriptorSetLayoutBinding()
				.setDescriptorCount(1)
				.setBinding(binding.myBindingIndex)
				.setDescriptorType(binding.myDescriptorType)
				.setStageFlags(binding.myShaderStages));
	}
	
	// Create layout
	if(!myLayout)
	{
		std::vector<vk::DescriptorBindingFlags> bindingFlags(
			layoutBindings.size(),
			vk::DescriptorBindingFlagBits::eUpdateAfterBind
		);
		
		vk::DescriptorSetLayoutBindingFlagsCreateInfo flagsInfo{};
		flagsInfo.setBindingFlags(bindingFlags);
		
		vk::DescriptorSetLayoutCreateInfo createInfo = vk::DescriptorSetLayoutCreateInfo().setBindings(layoutBindings).setFlags(vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool).setPNext(&flagsInfo);
		myLayout = VulkanContext::GetDevice()->createDescriptorSetLayout(createInfo);
	}
	
	// Create set
	vk::DescriptorSetAllocateInfo allocInfo = vk::DescriptorSetAllocateInfo()
		.setDescriptorPool(VulkanContext::GetDescriptorPool())
		.setSetLayouts(myLayout);

	
	mySet = VulkanContext::GetDevice()->allocateDescriptorSets(allocInfo).front();
}

void VulkanDescriptorSet::UpdateDescriptors()
{
	List<vk::WriteDescriptorSet> setWrites{};
	setWrites.Reserve(16);
	
	List<vk::DescriptorBufferInfo> bufferInfos{};
	bufferInfos.Reserve(myBuffers.size());
	
	List<vk::DescriptorImageInfo> imageInfos{};
	imageInfos.Reserve(mySampledImages.size());

	for(const BindingData<const IGPUBuffer*>& binding : myBuffers)
	{
		bufferInfos.Emplace()
			.setOffset(0)
			.setBuffer(binding.myData->GetBuffer()->GetAPIResource())
			.setRange(VK_WHOLE_SIZE);
		setWrites.Emplace()
			.setDescriptorType(binding.myDescriptorType)
			.setDstBinding(binding.myBindingIndex)
			.setBufferInfo(bufferInfos.Last());
	}
	
	for(const BindingData<const VulkanImage*>& binding : mySampledImages)
	{
		imageInfos.Emplace()
			.setSampler(binding.mySampler)
			.setImageView(binding.myData->GetImageView())
			.setImageLayout(binding.myImageLayout);

		setWrites.Emplace()
			.setDstBinding(binding.myBindingIndex)
			.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
			.setImageInfo(imageInfos.Last());
	}
	
	for (const BindingData<vk::Sampler>& binding : mySamplers)
	{
		vk::DescriptorImageInfo samplerInfo{};
		samplerInfo.setSampler(binding.myData);
		setWrites.Emplace()
			.setDstBinding(binding.myBindingIndex)
			.setDescriptorType(binding.myDescriptorType)
			.setImageInfo(samplerInfo);
	}
	
	// Update set
	for(vk::WriteDescriptorSet& write : setWrites)
	{
		write.setDstSet(mySet);
	}
	VulkanContext::GetDevice()->updateDescriptorSets(setWrites, {});
}
