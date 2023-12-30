#include "EnginePch.h"
#include "VulkanBuffer.h"
#include "VulkanAllocator.h"
#include <vma/vk_mem_alloc.h>

vk::Buffer VulkanBuffer::GetAPIResource() const
{
	return myBuffer;
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
