#include "EnginePch.h"
#include "VulkanBuffer.h"
#include "VulkanAllocator.h"
#include <vma/vk_mem_alloc.h>

#include "Engine.h"
#include "Rendering/RenderSystem.h"

vk::Buffer VulkanBuffer::GetAPIResource() const
{
	return myBuffer;
}

void VulkanBuffer::SetData(const void* inData, const size_t inSize, uint inOffset)
{
	check(GetSize() >= inSize + inOffset);

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
	return myCreateInfo.size;
}

const vk::BufferCreateInfo& VulkanBuffer::GetCreateInfo() const
{
	return myCreateInfo;
}

VmaMemoryUsage VulkanBuffer::GetVmaMemoryUsage() const
{
	return myMemoryUsage;	
}

bool VulkanBuffer::IsMappable() const
{
	return myIsMappingAllowed;
}

const std::string& VulkanBuffer::GetName() const
{
#if DEBUG
	return myName;	
#else
	return "";
#endif
}

void VulkanBuffer::UploadMapped(const void* inData, size_t inSize, uint inOffset)
{
	char* ptr = static_cast<char*>(Map());
	memcpy(ptr + inOffset, inData, inSize);
	Unmap();
}

void VulkanBuffer::UploadStaged(const void* inData, size_t inSize, uint inOffset)
{
	VulkanBuffer* stagingBuffer = VulkanAllocator::AllocateBuffer_TS("VulkanBuffer-Staging", VulkanBuffer::StagingCreateInfo(inSize), VMA_MEMORY_USAGE_AUTO, true);
	stagingBuffer->SetData(inData, inSize);

	Engine::GetEngineSystem<RenderSystem>().AddUploadCommand_TS(this, [this, stagingBuffer, inSize, inOffset](vk::CommandBuffer inCommandBuffer)
	{
		  vk::BufferCopy copyRegion = vk::BufferCopy().setSize(inSize).setDstOffset(inOffset);
		  inCommandBuffer.copyBuffer(stagingBuffer->GetAPIResource(), GetAPIResource(), { copyRegion });
	});

	VulkanAllocator::DestroyBuffer_TS(stagingBuffer);
}