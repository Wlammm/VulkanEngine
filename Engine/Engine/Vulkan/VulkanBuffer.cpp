#include "EnginePch.h"
#include "VulkanBuffer.h"
#include "VulkanAllocator.h"
#include <vma/vk_mem_alloc.h>

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
