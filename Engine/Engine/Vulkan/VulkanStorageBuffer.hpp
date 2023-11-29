#pragma once

#include "VulkanContext.h"
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanBuffer.h"

class IVulkanStorageBuffer
{
public:
	virtual vk::ShaderStageFlags GetShaderStageFlags() const = 0;
	virtual uint GetBindingIndex() const = 0;
	virtual vk::Buffer GetBuffer() const = 0;
};

template<typename T>
class VulkanStorageBuffer : public IVulkanStorageBuffer
{
public:
	VulkanStorageBuffer(const vk::ShaderStageFlags inShaderStageFlags, const uint inBindingIndex)
		: myShaderStages{ inShaderStageFlags }
		, myBindingIndex{ inBindingIndex }
	{	}

	~VulkanStorageBuffer()
	{
		InvalidateBuffer();
	}

	vk::ShaderStageFlags GetShaderStageFlags() const override final
	{
		return myShaderStages;
	}

	vk::Buffer GetBuffer() const override final
	{
		return *myBuffer;
	}

	uint GetBindingIndex() const override final
	{
		return myBindingIndex;
	}

	void SetData(const T& inData)
	{
		SetData({ inData });
	}

	void SetData(const List<T>& inData)
	{
		// TODO: this should probably not resize everytime it doesnt match. But instead send in a length to the shader.
		if(sizeof(T) * inData.size() != mySize)
		{
			InvalidateBuffer();
			CreateBuffer(sizeof(T) * inData.size());
		}

		void* data = myBuffer->Map();
		memcpy(data, inData.data(), sizeof(T) * inData.size());
		myBuffer->Unmap();
	}

private:
	void CreateBuffer(const size_t inSize)
	{
		mySize = inSize;
		vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
			.setSize(inSize)
			.setUsage(vk::BufferUsageFlagBits::eStorageBuffer)
			.setSharingMode(VULKAN_HPP_NAMESPACE::SharingMode::eExclusive);

		myBuffer = VulkanContext::GetAllocator().AllocateBuffer("StorageBuffer", createInfo, VMA_MEMORY_USAGE_AUTO);
	}

	void InvalidateBuffer()
	{
		if (!myBuffer)
			return;

		VulkanContext::GetAllocator().DestroyBuffer(myBuffer);
		myBuffer = nullptr;
	}

private:
	vk::ShaderStageFlags myShaderStages;
	uint myBindingIndex;
	VulkanBuffer* myBuffer = nullptr;
	size_t mySize;
};