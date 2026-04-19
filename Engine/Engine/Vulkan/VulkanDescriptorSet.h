#pragma once
#include "VulkanDynamicBuffer.hpp"
#include "Containers/IGPUAccelerationStructure.hpp"

class ResizableBuffer;
class VulkanDescriptorSet
{
public:
	VulkanDescriptorSet();
	VulkanDescriptorSet(vk::DescriptorSetLayout inLayout);
	~VulkanDescriptorSet();

	vk::DescriptorSetLayout GetLayout() const;
	vk::DescriptorSet GetSet() const;

	// Add all bindings first, then call Build().
	void BindBuffer(const class IGPUBuffer* inBuffer, vk::ShaderStageFlags inShaderStages, uint inBindingIndex, vk::DescriptorType inDescriptorType, vk::AccessFlags inAccessFlags);
	void BindSampler(vk::Sampler inSampler, vk::ShaderStageFlags inShaderStages, uint inBindingIndex);
	void BindImage(const class VulkanImage* inImage, const vk::Sampler inSampler, const uint inBinding, const vk::ShaderStageFlags inShaderFlags, vk::AccessFlags inAccessFlags, const vk::ImageLayout inImageLayout = vk::ImageLayout::eReadOnlyOptimal);
	void BindAccelerationStructure(const IGPUAccelerationStructure* inAS, vk::ShaderStageFlags inShaderStages, uint inBindingIndex, vk::AccessFlags inAccessFlags);
	
	void Build();

	void Rebuild();
	
	const List<ResourceUsage>& GetResourceUsages() const;
	
private:
	void BuildLayout();
	void AllocateDescriptorSet();
	void DestroyDescriptorSet();
	void UpdateDescriptors();
	
	int GetNumDescriptors();
	
	template<typename Type>
	struct BindingData
	{
		vk::ShaderStageFlags myShaderStages;
		uint myBindingIndex;
		vk::DescriptorType myDescriptorType;
		vk::AccessFlags myAccessFlags;
		
		vk::ImageLayout myImageLayout; // Only required for images.
		vk::Sampler mySampler; // Only required for images.
		
		Type myData;
	};
	
	List<BindingData<const IGPUBuffer*>> myBuffers{};
	List<BindingData<const VulkanImage*>> mySampledImages{};
	List<BindingData<vk::Sampler>> mySamplers{};
	List<BindingData<const IGPUAccelerationStructure*>> myAccelerationStructures{};
	
	List<ResourceUsage> myResourceUsages{};
	
	
	// If a layout is passed in as constructor we do not want to handle deletion of it.
	bool myUsesSharedLayout = false;
	vk::DescriptorSetLayout myLayout;
	vk::DescriptorSet mySet;
};
