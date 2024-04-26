#include "EnginePch.h"
#include "VulkanBuffer.h"
#include "VulkanAllocator.h"
#include <vma/vk_mem_alloc.h>
#include "ECS/Systems/RenderSystem.h"

vk::Buffer VulkanBuffer::GetAPIResource() const
{
	return myBuffer;
}

void VulkanBuffer::SetData(void* inData, const size_t inSize, uint inOffset)
{
	check(inSize <= mySize);

	if(myIsMappingAllowed)
	{
		UploadMapped(inData, inSize, inOffset);
	}
	else
	{
		UploadStaged(inData, inSize, inOffset);
	}
}

void* VulkanBuffer::Map()
{
	void* ptr;
	VkResult result = vmaMapMemory(VulkanAllocator::GetVMAAllocator(), myAllocation, &ptr);
	check(result == VK_SUCCESS);
	return ptr;
}

void VulkanBuffer::Unmap()
{
	vmaUnmapMemory(VulkanAllocator::GetVMAAllocator(), myAllocation);
}

size_t VulkanBuffer::GetSize() const
{
	return mySize;
}

void VulkanBuffer::UploadMapped(void* inData, size_t inSize, uint inOffset)
{
	void* ptr = Map();
	memcpy(ptr, inData, inSize);
	Unmap();
}

void VulkanBuffer::UploadStaged(void* inData, size_t inSize, uint inOffset)
{
	VulkanBuffer* stagingBuffer = VulkanAllocator::AllocateBuffer_TS("VulkanBuffer-Staging", VulkanBuffer::StagingCreateInfo(inSize), VMA_MEMORY_USAGE_AUTO, true);
	stagingBuffer->SetData(inData, inSize);

	RenderSystem::AddUploadCommand_TS(this, [this, stagingBuffer, inSize](vk::CommandBuffer inCommandBuffer)
	{
		  vk::BufferCopy copyRegion = vk::BufferCopy().setSize(inSize);
		  inCommandBuffer.copyBuffer(stagingBuffer->GetAPIResource(), GetAPIResource(), { copyRegion });
	});

	VulkanAllocator::DestroyBuffer_TS(stagingBuffer);
}