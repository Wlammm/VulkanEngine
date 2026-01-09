#include "EnginePch.h"
#include "VulkanBuffer.h"

#include <stacktrace>

#include "VulkanAllocator.h"
#include <vma/vk_mem_alloc.h>

#include "VulkanCommandBuffer.h"
#include "Engine/Engine.h"
#include "Engine/Rendering/RenderSystem.h"
#include "Staging/StagingSystem.h"

vk::Buffer VulkanBuffer::GetAPIResource() const
{
	return myBuffer;
}

void VulkanBuffer::CopyDataFromBuffer(VulkanBuffer* inStagingBuffer, const uint inSize, uint inOffset)
{
	check(GetSize() >= inSize + inOffset);

	VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
	vk::BufferCopy copyRegion = vk::BufferCopy().setSize(inSize).setDstOffset(inOffset);
	commandBuffer->GetAPIResource().copyBuffer(inStagingBuffer->GetAPIResource(), GetAPIResource(), { copyRegion });
	
	List<ResourceUsage> resourceUsages{};
	resourceUsages.Emplace().SetToBuffer(this, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferWrite);
	resourceUsages.Emplace().SetToBuffer(inStagingBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferRead);
	
	RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer, resourceUsages);
}

void VulkanBuffer::SetData(const void* inData, const uint inSize, uint inOffset)
{
	ZoneScoped;
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

uint VulkanBuffer::GetSize() const
{
	return static_cast<uint>(myCreateInfo.size);
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

#if DEBUG
const std::string& VulkanBuffer::GetName() const
{
	return myName;	
}
#else
std::string VulkanBuffer::GetName() const
{
	return "";
}
#endif

void* VulkanBuffer::GetPtr() const
{
	check(myIsMappingAllowed && myPtr);
	return myPtr;
}

void VulkanBuffer::UploadMapped(const void* inData, uint inSize, uint inOffset)
{
	ZoneScoped;
	check(myPtr);
	memcpy((char*)myPtr + inOffset, inData, inSize);
}

void VulkanBuffer::UploadStaged(const void* inData, uint inSize, uint inOffset)
{
	ZoneScoped;
	
	StagingBuffer stagingBuffer = StagingSystem::GetStagingBufferWithSize_TS(inSize);
	stagingBuffer.SetData(inData, inSize);
	
	VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
	vk::BufferCopy copyRegion = vk::BufferCopy().setSize(inSize).setDstOffset(inOffset).setSrcOffset(stagingBuffer.GetOffset());
	commandBuffer->GetAPIResource().copyBuffer(stagingBuffer.GetUnderlyingBuffer()->GetAPIResource(), GetAPIResource(), { copyRegion });
	
	List<ResourceUsage> resourceUsages{};
	resourceUsages.Emplace().SetToBuffer(stagingBuffer.GetUnderlyingBuffer(), vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferRead);
	resourceUsages.Emplace().SetToBuffer(this, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferWrite);
	
	RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer, resourceUsages);
}