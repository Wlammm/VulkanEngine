#pragma once
#include "VulkanDynamicBuffer.hpp"

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
	void BindBuffer(const class VulkanBuffer* inBuffer, vk::ShaderStageFlags inShaderStages, uint inBindingIndex, vk::DescriptorType inDescriptorType);
	void BindBuffer(const class ResizableBuffer* inBuffer, vk::ShaderStageFlags inShaderStages, uint inBindingIndex, vk::DescriptorType inDescriptorType);
	void BindBuffer(const class IGPUList* inBuffer, vk::ShaderStageFlags inShaderStages, uint inBindingIndex, vk::DescriptorType inDescriptorType);
	
	void BindImage(const class VulkanImage* inImage, const vk::Sampler inSampler, const uint inBinding, const vk::ShaderStageFlags inShaderFlags, const vk::ImageLayout inImageLayout = vk::ImageLayout::eReadOnlyOptimal);
	
	void Build();

	void Rebuild();
	
private:
	template<typename Type>
	struct BindingData
	{
		vk::ShaderStageFlags myShaderStages;
		uint myBindingIndex;
		vk::DescriptorType myDescriptorType;
		
		vk::ImageLayout myImageLayout; // Only required for images.
		vk::Sampler mySampler; // Only required for images.
		
		Type myData;
	};
	
	List<BindingData<const VulkanBuffer*>> myBuffers{};
	// TODO: Remove old resizable buffer.
	List<BindingData<const ResizableBuffer*>> myResizableBuffer{};
	List<BindingData<const IGPUList*>> myResizableBuffers{};
	List<BindingData<const VulkanImage*>> myImages{};
	
	// If a layout is passed in as constructor we do not want to handle deletion of it.
	bool myUsesSharedLayout = false;
	vk::DescriptorSetLayout myLayout;
	vk::DescriptorSet mySet;
};
