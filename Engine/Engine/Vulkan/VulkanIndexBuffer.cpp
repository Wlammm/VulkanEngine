#include "EnginePch.h"
#include "VulkanIndexBuffer.h"
#include "VulkanContext.h"
#include "VulkanAllocator.h"
#include "VulkanBuffer.h"
#include "VulkanDevice.h"

VulkanIndexBuffer::VulkanIndexBuffer(const List<uint>& inIndices)
{
	vk::BufferCreateInfo stagingCreateInfo = vk::BufferCreateInfo()
		.setSize(inIndices.size() * sizeof(uint))
		.setUsage(vk::BufferUsageFlagBits::eTransferSrc)
		.setSharingMode(vk::SharingMode::eExclusive);

	VulkanBuffer* stagingBuffer = VulkanContext::GetAllocator().AllocateBuffer("Index staging buffer", stagingCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU);
	void* stagingMemory = stagingBuffer->Map();
	memcpy(stagingMemory, inIndices.data(), inIndices.size() * sizeof(uint));
	stagingBuffer->Unmap();

	vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
		.setSize(inIndices.size() * sizeof(uint))
		.setUsage(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer);
	myBuffer = VulkanContext::GetAllocator().AllocateBuffer("IndexBuffer", createInfo, VMA_MEMORY_USAGE_GPU_ONLY);

	vk::CommandBuffer copyCmd = VulkanContext::GetDevice().CreateCommandBuffer(true);

	vk::BufferCopy copyRegion = vk::BufferCopy().setSize(inIndices.size() * sizeof(uint));
	copyCmd.copyBuffer(*stagingBuffer, *myBuffer, { copyRegion });

	VulkanContext::GetDevice().FlushCommandBuffer(copyCmd);
	VulkanContext::GetAllocator().DestroyBuffer(stagingBuffer);
}

VulkanIndexBuffer::~VulkanIndexBuffer()
{
	VulkanContext::GetAllocator().DestroyBuffer(myBuffer);
	myBuffer = nullptr;
}

void VulkanIndexBuffer::Bind(vk::CommandBuffer inCommandBuffer)
{
	inCommandBuffer.bindIndexBuffer(*myBuffer, 0, vk::IndexType::eUint32);
}
