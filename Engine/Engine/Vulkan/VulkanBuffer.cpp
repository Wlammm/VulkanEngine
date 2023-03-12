#include "EnginePch.h"
#include "VulkanBuffer.h"
#include "VulkanContext.h"
#include <vma/vk_mem_alloc.h>
#include "VulkanAllocator.h"

void* VulkanBuffer::Map()
{
	void* ptr;
	vmaMapMemory(VulkanContext::GetAllocator(), myAllocation, &ptr);
	return ptr;
}

void VulkanBuffer::Unmap()
{
	vmaUnmapMemory(VulkanContext::GetAllocator(), myAllocation);
}
