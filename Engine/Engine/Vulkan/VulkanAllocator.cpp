#include "EnginePch.h"
#define VMA_IMPLEMENTATION
#define VMA_DEBUG_LOG
#include <vma/vk_mem_alloc.h>

#include "VulkanAllocator.h"

#include "ResizableBuffer.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"
#include "VulkanContext.h"

VulkanAllocator::VulkanAllocator(vk::Instance inInstance, const VulkanPhysicalDevice& inPhysicalDevice, const VulkanDevice& inDevice)
{
	myInstance = this;

	VmaAllocatorCreateInfo createInfo{};
	createInfo.vulkanApiVersion = USED_VULKAN_VERSION;
	createInfo.instance = inInstance;
	createInfo.physicalDevice = inPhysicalDevice.GetPhysicalDevice();
	createInfo.device = inDevice.GetDevice();

	vmaCreateAllocator(&createInfo, &myAllocator);
}

VulkanAllocator::~VulkanAllocator()
{
	VulkanContext::GetDevice()->waitIdle();
	for(int i = 0; i < VulkanAllocator::DestructionFrameDelay; ++i)
	{
		TickBufferDeletes();
		TickImageDeletes();
		TickDelegateDeletes();
	}

#if DEBUG
	myAllocatedNames.Lock();
	for(const std::string& allocationName : myAllocatedNames)
	{
		LOG_ERROR("Unallocated buffer: %s", allocationName.c_str());
	}
	myAllocatedNames.Unlock();
#endif

	vmaDestroyAllocator(myAllocator);
	myInstance = nullptr;
}

void VulkanAllocator::Tick()
{
	TickBufferDeletes();
	TickImageDeletes();
	TickDelegateDeletes();
}

VulkanBuffer* VulkanAllocator::AllocateBuffer_TS(const std::string& inName, const vk::BufferCreateInfo& inCreateInfo, VmaMemoryUsage inUsage, bool inMappable)
{
	check(inCreateInfo.size != 0 && "Cannot allocate buffer with 0 size.");
	VulkanBuffer* outBuffer = new VulkanBuffer();
	VmaAllocationCreateInfo allocCreateInfo{};
	allocCreateInfo.usage = inUsage;
	if(inMappable)
		allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
	VkBuffer buffer;
	VkBufferCreateInfo info = inCreateInfo;
	VkResult result = vmaCreateBuffer(myInstance->myAllocator, &info, &allocCreateInfo, &buffer, &outBuffer->myAllocation, nullptr);
	check(result == VK_SUCCESS);

	outBuffer->myBuffer = buffer;
	outBuffer->myIsMappingAllowed = inMappable;
	outBuffer->myCreateInfo = inCreateInfo;
	outBuffer->myMemoryUsage = inUsage;

	if(inMappable)
	{
		result = vmaMapMemory(myInstance->myAllocator, outBuffer->myAllocation, &outBuffer->myPtr);
		check(result == VK_SUCCESS);
	}

#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
		.setObjectHandle(VulkanContext::GetVulkanHandle(outBuffer->GetAPIResource()))
		.setPObjectName(inName.c_str())
		.setObjectType(vk::ObjectType::eBuffer));
	outBuffer->myName = inName;
	myInstance->myAllocatedNames.Add(inName);
#endif

	return outBuffer;
}

void VulkanAllocator::DestroyBuffer_TS(VulkanBuffer* inBuffer)
{
	myInstance->myBufferDeleteData.Add({ VulkanAllocator::DestructionFrameDelay, inBuffer });
}

void VulkanAllocator::DestroyBuffer_TS(ResizableBuffer* inBuffer)
{
	myInstance->myResizableBufferDeleteData.Add({VulkanAllocator::DestructionFrameDelay, inBuffer });
}

void VulkanAllocator::QueueDestroyCommand(const Delegate<void()>& inCommand)
{
	myInstance->myDelegateDeletes.Add({VulkanAllocator::DestructionFrameDelay, inCommand });
}

VulkanImage* VulkanAllocator::AllocateImage_TS(const std::string& inName, const vk::ImageCreateInfo& inCreateInfo, VmaMemoryUsage inUsage)
{
	VulkanImage* outImage = new VulkanImage();
	VmaAllocationCreateInfo allocCreateInfo{};
	allocCreateInfo.usage = inUsage;

	VkImage image;
	const VkImageCreateInfo info = inCreateInfo;
	VkResult result = vmaCreateImage(myInstance->myAllocator, &info, &allocCreateInfo, &image, &outImage->myAllocation, nullptr);
	check(result == VK_SUCCESS);
	outImage->myImage = image;
	outImage->myFormat = inCreateInfo.format;
	outImage->mySize = glm::vec2(inCreateInfo.extent.width, inCreateInfo.extent.height);
	outImage->myNumMipLevels = inCreateInfo.mipLevels;
	outImage->myNumMSAASamples = inCreateInfo.samples;
#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
		.setObjectHandle(VulkanContext::GetVulkanHandle(outImage->GetAPIResource()))
		.setPObjectName(inName.c_str())
		.setObjectType(vk::ObjectType::eImage));
	outImage->myName = inName;
	myInstance->myAllocatedNames.Add(inName);
#endif

	return outImage;
}

void VulkanAllocator::DestroyImage_TS(VulkanImage* inImage)
{
	myInstance->myImageDeleteData.Add({ VulkanAllocator::DestructionFrameDelay, inImage });
}

VmaAllocator VulkanAllocator::GetVMAAllocator()
{
	return myInstance->myAllocator;
}

void VulkanAllocator::DestroyBufferInternal(VulkanBuffer* inBuffer)
{
	ZoneScoped;
	ZoneText(std::to_string(inBuffer->GetSize()).c_str(), 14);
#if DEBUG
	myInstance->myAllocatedNames.Remove(inBuffer->myName);
#endif

	if(inBuffer->myIsMappingAllowed)
		vmaUnmapMemory(myInstance->myAllocator, inBuffer->myAllocation);

	vmaDestroyBuffer(myInstance->myAllocator, inBuffer->myBuffer, inBuffer->myAllocation);
	del(inBuffer);
}

void VulkanAllocator::DestroyImageInternal(VulkanImage* inImage)
{
	ZoneScoped;
#if DEBUG
	myInstance->myAllocatedNames.Remove(inImage->myName);
#endif
	vmaDestroyImage(myInstance->myAllocator, inImage->myImage, inImage->myAllocation);
	del(inImage);
}

void VulkanAllocator::TickBufferDeletes()
{
	ZoneScoped;
	myBufferDeleteData.Lock();
	for (int i = myBufferDeleteData.size() - 1; i >= 0; --i)
	{
		myBufferDeleteData[i].myFramesUntilDelete--;
		if (myBufferDeleteData[i].myFramesUntilDelete <= 0)
		{
			DestroyBufferInternal(myBufferDeleteData[i].myData);
			myBufferDeleteData.RemoveIndex(i);
		}
	}
	myBufferDeleteData.Unlock();

	myResizableBufferDeleteData.Lock();
	for (int i = myResizableBufferDeleteData.size() - 1; i >= 0; --i)
	{
		myResizableBufferDeleteData[i].myFramesUntilDelete--;
		if (myResizableBufferDeleteData[i].myFramesUntilDelete <= 0)
		{
			DestroyBufferInternal(myResizableBufferDeleteData[i].myData->GetBuffer());
			del(myResizableBufferDeleteData[i].myData);
			myResizableBufferDeleteData.RemoveIndex(i);
		}
	}
	myResizableBufferDeleteData.Unlock();
}

void VulkanAllocator::TickImageDeletes()
{
	ZoneScoped;
	myImageDeleteData.Lock();
	for (int i = myImageDeleteData.size() - 1; i >= 0; --i)
	{
		myImageDeleteData[i].myFramesUntilDelete--;
		if (myImageDeleteData[i].myFramesUntilDelete <= 0)
		{
			DestroyImageInternal(myImageDeleteData[i].myData);
			myImageDeleteData.RemoveIndex(i);
		}
	}
	myImageDeleteData.Unlock();
}

void VulkanAllocator::TickDelegateDeletes()
{
	ZoneScoped;
	myDelegateDeletes.Lock();
	for (int i = myDelegateDeletes.size() - 1; i >= 0; --i)
	{
		myDelegateDeletes[i].myFramesUntilDelete--;
		if (myDelegateDeletes[i].myFramesUntilDelete <= 0)
		{
			myDelegateDeletes[i].myDelegate();
			myDelegateDeletes.RemoveIndex(i);
		}
	}
	myDelegateDeletes.Unlock();
}
