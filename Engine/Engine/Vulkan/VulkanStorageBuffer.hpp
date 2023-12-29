#pragma once
#include "VulkanStorageBufferFwd.hpp"
#include "VulkanContext.h"
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanBuffer.h"
#include "VulkanAllocator.h"
#include "ECS/Systems/RenderSystem.h"

template<typename T>
VulkanStorageBuffer<T>::VulkanStorageBuffer(const vk::ShaderStageFlags inShaderStageFlags, const uint inBindingIndex)
	: myShaderStages{ inShaderStageFlags }
	, myBindingIndex{ inBindingIndex }
{
	CreateBuffers(sizeof(T));
}

template<typename T>
VulkanStorageBuffer<T>::~VulkanStorageBuffer()
{
	InvalidateBuffer();
}

template<typename T>
vk::ShaderStageFlags VulkanStorageBuffer<T>::GetShaderStageFlags() const
{
	return myShaderStages;
}

template<typename T>
vk::Buffer VulkanStorageBuffer<T>::GetBuffer() const
{
	return *myBuffer;
}

template<typename T>
size_t VulkanStorageBuffer<T>::GetBufferSize() const
{
	return mySize;
}

template<typename T>
uint VulkanStorageBuffer<T>::GetBindingIndex() const
{
	return myBindingIndex;
}

template<typename T>
void VulkanStorageBuffer<T>::SetData(const T& inData)
{
	vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
		.setSize(sizeof(T))
		.setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst)
		.setSharingMode(VULKAN_HPP_NAMESPACE::SharingMode::eExclusive);
	createInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
	VulkanBuffer* stagingBuffer = VulkanAllocator::AllocateBuffer_TS("Staging StorageBuffer", createInfo, VMA_MEMORY_USAGE_CPU_TO_GPU);

	check(!std::is_pointer<T>::value);
	check(sizeof(T) <= mySize);
	void* data = stagingBuffer->Map();
	memcpy(data, &inData, sizeof(T));
	stagingBuffer->Unmap();

	RenderSystem::AddUploadCommand_TS([this, stagingBuffer](vk::CommandBuffer inCommandBuffer)
		{
			vk::BufferCopy copyRegion = vk::BufferCopy().setSize(sizeof(T));
			inCommandBuffer.copyBuffer(*stagingBuffer, *myBuffer, { copyRegion });
			VulkanAllocator::DestroyBuffer_TS(stagingBuffer);
		});
}

template<typename T>
void VulkanStorageBuffer<T>::CreateBuffers(const size_t inSize)
{
	mySize = inSize;
	vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
		.setSize(inSize)
		.setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst)
		.setSharingMode(VULKAN_HPP_NAMESPACE::SharingMode::eExclusive);

	myBuffer = VulkanAllocator::AllocateBuffer_TS("StorageBuffer", createInfo, VMA_MEMORY_USAGE_GPU_ONLY);
}

template<typename T>
void VulkanStorageBuffer<T>::InvalidateBuffer()
{
	if (!myBuffer)
		return;

	VulkanAllocator::DestroyBuffer_TS(myBuffer);
	myBuffer = nullptr;
}
