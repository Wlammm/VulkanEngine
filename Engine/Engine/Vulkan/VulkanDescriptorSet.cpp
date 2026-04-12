#include "EnginePch.h"
#include "VulkanDescriptorSet.h"
#include <set>

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

	for (const BindingData<const IGPUAccelerationStructure*>& binding : myAccelerationStructures)
	{
		if (binding.myData && binding.myData->GetOnRebuilt())
			binding.myData->GetOnRebuilt()->UnBind(&VulkanDescriptorSet::Rebuild, this);
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
	// If this binding index already exists (e.g. shared between VS and PS), just merge stage flags.
	for (BindingData<const IGPUBuffer*>& existing : myBuffers)
	{
		if (existing.myBindingIndex == inBindingIndex)
		{
			existing.myShaderStages |= inShaderStages;
			return;
		}
	}

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
	for (BindingData<vk::Sampler>& existing : mySamplers)
	{
		if (existing.myBindingIndex == inBindingIndex)
		{
			existing.myShaderStages |= inShaderStages;
			return;
		}
	}

	BindingData<vk::Sampler> data{};
	data.myData = inSampler;
	data.myBindingIndex = inBindingIndex;
	data.myShaderStages = inShaderStages;
	data.myDescriptorType = vk::DescriptorType::eSampler;
	mySamplers.Add(data);
}

void VulkanDescriptorSet::BindAccelerationStructure(const IGPUAccelerationStructure* inAS, vk::ShaderStageFlags inShaderStages, uint inBindingIndex)
{
	for (BindingData<const IGPUAccelerationStructure*>& existing : myAccelerationStructures)
	{
		if (existing.myBindingIndex == inBindingIndex)
		{
			existing.myShaderStages |= inShaderStages;
			existing.myData = inAS;
			return;
		}
	}

	BindingData<const IGPUAccelerationStructure*> data{};
	data.myData = inAS;
	data.myShaderStages = inShaderStages;
	data.myBindingIndex = inBindingIndex;
	data.myDescriptorType = vk::DescriptorType::eAccelerationStructureKHR;
	myAccelerationStructures.Add(data);

	if (inAS && inAS->GetOnRebuilt() && !inAS->GetOnRebuilt()->IsBound(Delegate<void()>(&VulkanDescriptorSet::Rebuild, this)))
		inAS->GetOnRebuilt()->Bind(&VulkanDescriptorSet::Rebuild, this);
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
		// Skip if an image binding already occupies this slot (e.g. combined image sampler
		// set up manually in SetupDescriptors while SPIR-V reflection also surfaces the
		// sampler component as a separate eSampler descriptor at the same index).
		bool conflict = false;
		for (const vk::DescriptorSetLayoutBinding& existing : layoutBindings)
		{
			if (existing.binding == binding.myBindingIndex)
			{
				conflict = true;
				break;
			}
		}
		if (conflict)
			continue;

		layoutBindings.Add(
			vk::DescriptorSetLayoutBinding()
				.setDescriptorCount(1)
				.setBinding(binding.myBindingIndex)
				.setDescriptorType(binding.myDescriptorType)
				.setStageFlags(binding.myShaderStages));
	}

	for (const BindingData<const IGPUAccelerationStructure*>& binding : myAccelerationStructures)
	{
		layoutBindings.Emplace()
			.setDescriptorCount(1)
			.setDescriptorType(vk::DescriptorType::eAccelerationStructureKHR)
			.setStageFlags(binding.myShaderStages)
			.setBinding(binding.myBindingIndex);
	}

	// Create layout
	if(!myLayout)
	{
		// AS bindings may be null initially (not yet built) — mark them as partially bound.
		std::set<uint32_t> asBindingIndices;
		for (const BindingData<const IGPUAccelerationStructure*>& binding : myAccelerationStructures)
			asBindingIndices.insert(binding.myBindingIndex);

		std::vector<vk::DescriptorBindingFlags> bindingFlags;
		bindingFlags.reserve(layoutBindings.size());
		for (const vk::DescriptorSetLayoutBinding& lb : layoutBindings)
		{
			vk::DescriptorBindingFlags flags = vk::DescriptorBindingFlagBits::eUpdateAfterBind;
			if (asBindingIndices.count(lb.binding))
				flags |= vk::DescriptorBindingFlagBits::ePartiallyBound;
			bindingFlags.push_back(flags);
		}
		
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
		// Skip if a combined image sampler already occupies this binding slot.
		bool conflict = false;
		for (const BindingData<const VulkanImage*>& img : mySampledImages)
		{
			if (img.myBindingIndex == binding.myBindingIndex)
			{
				conflict = true;
				break;
			}
		}
		if (conflict)
			continue;

		vk::DescriptorImageInfo samplerInfo{};
		samplerInfo.setSampler(binding.myData);
		setWrites.Emplace()
			.setDstBinding(binding.myBindingIndex)
			.setDescriptorType(binding.myDescriptorType)
			.setImageInfo(samplerInfo);
	}
	
	// Acceleration structures need pNext chaining. Pre-reserve so the List never reallocates
	// (keeping &w.handle stable for the ArrayProxy pointer stored in w.info).
	// Skip bindings whose AS is not yet built (ePartiallyBound allows null slots in the layout).
	struct ASWrite { vk::AccelerationStructureKHR handle; vk::WriteDescriptorSetAccelerationStructureKHR info; uint bindingIndex; };
	List<ASWrite> asWrites;
	asWrites.Reserve(myAccelerationStructures.size());
	for (const BindingData<const IGPUAccelerationStructure*>& binding : myAccelerationStructures)
	{
		if (!binding.myData)
			continue;
		vk::AccelerationStructureKHR handle = binding.myData->GetAccelerationStructure();
		if (!handle)
			continue;
		ASWrite& w = asWrites.Emplace();
		w.handle       = handle;
		w.bindingIndex = binding.myBindingIndex;
		w.info.setAccelerationStructures(w.handle);
	}
	List<vk::WriteDescriptorSet> asDescriptorWrites;
	asDescriptorWrites.Reserve(asWrites.size());
	for (ASWrite& w : asWrites)
	{
		asDescriptorWrites.Emplace()
			.setDstSet(mySet)
			.setDstBinding(w.bindingIndex)
			.setDescriptorType(vk::DescriptorType::eAccelerationStructureKHR)
			.setDescriptorCount(1)
			.setPNext(&w.info);
	}

	// Update set
	for(vk::WriteDescriptorSet& write : setWrites)
	{
		write.setDstSet(mySet);
	}
	VulkanContext::GetDevice()->updateDescriptorSets(setWrites, {});
	VulkanContext::GetDevice()->updateDescriptorSets(asDescriptorWrites, {});
}
