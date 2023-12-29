#include "EnginePch.h"
#include "VulkanBuffer.h"
#include "VulkanContext.h"
#include <vma/vk_mem_alloc.h>
#include "VulkanAllocator.h"

void* VulkanBuffer::Map()
{
	void* ptr;
	VkResult result = vmaMapMemory(VulkanContext::GetAllocator(), myAllocation, &ptr);
	check(result == VK_SUCCESS);
	return ptr;
}

void VulkanBuffer::Unmap()
{
	vmaUnmapMemory(VulkanContext::GetAllocator(), myAllocation);
}
