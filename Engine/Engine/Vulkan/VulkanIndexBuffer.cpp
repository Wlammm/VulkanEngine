#include "EnginePch.h"
#include "VulkanIndexBuffer.h"
#include "VulkanContext.h"
#include "VulkanAllocator.h"
#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "Tracy/tracy/Tracy.hpp"

VulkanIndexBuffer::VulkanIndexBuffer(const List<uint>& inIndices)
{
	ZoneScoped;
	vk::BufferCreateInfo stagingCreateInfo = vk::BufferCreateInfo()
		.setSize(inIndices.size() * sizeof(uint))
		.setUsage(vk::BufferUsageFlagBits::eTransferSrc)
		.setSharingMode(vk::SharingMode::eExclusive);

	VulkanBuffer* stagingBuffer = VulkanAllocator::AllocateBuffer_TS("Index staging buffer", stagingCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU);
	void* stagingMemory = stagingBuffer->Map();
	memcpy(stagingMemory, inIndices.data(), inIndices.size() * sizeof(uint));
	stagingBuffer->Unmap();

	vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
		.setSize(inIndices.size() * sizeof(uint))
		.setUsage(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer);
	myBuffer = VulkanAllocator::AllocateBuffer_TS("IndexBuffer", createInfo, VMA_MEMORY_USAGE_GPU_ONLY);

	vk::CommandBuffer copyCmd = VulkanContext::GetDevice().CreateCommandBuffer(true);

	vk::BufferCopy copyRegion = vk::BufferCopy().setSize(inIndices.size() * sizeof(uint));
	copyCmd.copyBuffer(stagingBuffer->GetAPIResource(), myBuffer->GetAPIResource(), { copyRegion });

	VulkanContext::GetDevice().FlushCommandBuffer(copyCmd);
	VulkanAllocator::DestroyBuffer_TS(stagingBuffer);
}

VulkanIndexBuffer::~VulkanIndexBuffer()
{
	VulkanAllocator::DestroyBuffer_TS(myBuffer);
	myBuffer = nullptr;
}

void VulkanIndexBuffer::Bind(vk::CommandBuffer inCommandBuffer)
{
	inCommandBuffer.bindIndexBuffer(myBuffer->GetAPIResource(), 0, vk::IndexType::eUint32);
}