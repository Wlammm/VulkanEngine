#pragma once

#include "VulkanContext.h"
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"

class IVulkanUniformBuffer
{
public:
	virtual vk::ShaderStageFlags GetShaderStageFlags() const = 0;
	virtual uint GetBindingIndex() const = 0;
	virtual constexpr uint GetBufferSize() const = 0;
	virtual vk::Buffer GetBuffer(const uint inIndex) const = 0;
};

template<typename T>
class VulkanUniformBuffer : public IVulkanUniformBuffer
{
public:
	VulkanUniformBuffer(const vk::ShaderStageFlags inShaderStageFlags, const uint inBindingIndex)
		: myShaderStageFlags{ inShaderStageFlags }
		, myBindingIndex{ inBindingIndex }
	{
		const auto createInfo = vk::BufferCreateInfo().setSize(sizeof(T)).setUsage(vk::BufferUsageFlagBits::eUniformBuffer);

		for(uint i = 0; i < VulkanContext::GetSwapChain().GetFrameLag(); ++i)
		{
			myBuffers.Add(VulkanContext::GetDevice()->createBuffer(createInfo));

			vk::MemoryRequirements memoryRequirements; 
			VulkanContext::GetDevice()->getBufferMemoryRequirements(myBuffers[i], &memoryRequirements);

			auto memoryAllocInfo = vk::MemoryAllocateInfo().setAllocationSize(memoryRequirements.size).setMemoryTypeIndex(0);
			memoryAllocInfo.memoryTypeIndex = GetMemoryTypeIndexFromProperties(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
			myMemory.Add(VulkanContext::GetDevice()->allocateMemory(memoryAllocInfo));

			myMemoryPtrs.Add(VulkanContext::GetDevice()->mapMemory(myMemory[i], 0, VK_WHOLE_SIZE, vk::MemoryMapFlags()));
			VulkanContext::GetDevice()->bindBufferMemory(myBuffers[i], myMemory[i], 0);
		}
	}

	~VulkanUniformBuffer()
	{
		for(uint i = 0; i < VulkanContext::GetSwapChain().GetFrameLag(); ++i)
		{
			VulkanContext::GetDevice()->destroyBuffer(myBuffers[i]);
			VulkanContext::GetDevice()->unmapMemory(myMemory[i]);
			VulkanContext::GetDevice()->freeMemory(myMemory[i]);
		}
		myBuffers.Clear();
		myMemory.Clear();
		myMemoryPtrs.Clear();
	}

	T& Get()
	{
		return *(T*)myMemoryPtrs[VulkanContext::GetSwapChain().GetSwapChainIndex()];
	}

	vk::ShaderStageFlags GetShaderStageFlags() const override final
	{
		return myShaderStageFlags;
	}

	uint GetBindingIndex() const override final
	{
		return myBindingIndex;
	}

	constexpr uint GetBufferSize() const override final
	{
		return sizeof(T);
	}

	vk::Buffer GetBuffer(const uint inIndex) const override final
	{
		return myBuffers[inIndex];
	}

private:
	uint GetMemoryTypeIndexFromProperties(uint inTypeBits, const vk::MemoryPropertyFlags inRequirementsMask)
	{
		for(uint i = 0; i < VK_MAX_MEMORY_TYPES; ++i)
		{
			if((inTypeBits & 1) == 1)
			{
				if((VulkanContext::GetPhysicalDevice().GetMemoryProperties().memoryTypes[i].propertyFlags & inRequirementsMask) == inRequirementsMask)
				{
					return i;
				}
			}
			inTypeBits >>= 1;
		}
		THROW_IF(true, "Failed to find memory type.");
		return 0;
	}

private:
	List<vk::DeviceMemory> myMemory{};
	List<vk::Buffer> myBuffers{};
	List<void*> myMemoryPtrs{};

	vk::ShaderStageFlags myShaderStageFlags;
	uint myBindingIndex;
};