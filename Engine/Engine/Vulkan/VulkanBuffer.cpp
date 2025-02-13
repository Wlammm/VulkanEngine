#include "EnginePch.h"
#include "VulkanBuffer.h"

#include <stacktrace>

#include "VulkanAllocator.h"
#include <vma/vk_mem_alloc.h>

#include "Engine.h"
#include "Rendering/RenderSystem.h"
#include "Staging/StagingSystem.h"

vk::Buffer VulkanBuffer::GetAPIResource() const
{
	return myBuffer;
}

void VulkanBuffer::CopyDataFromBuffer(VulkanBuffer* inStagingBuffer, const uint inSize, uint inOffset)
{
	check(GetSize() >= inSize + inOffset);

	Engine::GetEngineSystem<RenderSystem>().AddUploadCommand_TS(this, [this, inStagingBuffer, inSize, inOffset](vk::CommandBuffer inCommandBuffer)
	{
		  vk::BufferCopy copyRegion = vk::BufferCopy().setSize(inSize).setDstOffset(inOffset);
		  inCommandBuffer.copyBuffer(inStagingBuffer->GetAPIResource(), GetAPIResource(), { copyRegion });
	});
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
	//VulkanBuffer* stagingBuffer = VulkanAllocator::AllocateBuffer_TS("VulkanBuffer::UploadStaged Staging buffer", VulkanBuffer::StagingCreateInfo(inSize), VMA_MEMORY_USAGE_AUTO, true);
	//stagingBuffer->SetData(inData, inSize);

	StagingSystem& stagingSystem = Engine::GetEngineSystem<StagingSystem>();
	StagingBuffer stagingBuffer = stagingSystem.GetStagingBufferWithSize(inSize);
	stagingBuffer.SetData(inData, inSize);

	Engine::GetEngineSystem<RenderSystem>().AddUploadCommand_TS(this, [this, stagingBuffer, inSize, inOffset](vk::CommandBuffer inCommandBuffer)
	{
		  vk::BufferCopy copyRegion = vk::BufferCopy().setSize(inSize).setDstOffset(inOffset).setSrcOffset(stagingBuffer.GetOffset());
		  inCommandBuffer.copyBuffer(stagingBuffer.GetUnderlyingBuffer()->GetAPIResource(), GetAPIResource(), { copyRegion });
	});
}