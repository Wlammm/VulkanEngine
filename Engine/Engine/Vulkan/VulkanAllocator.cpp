#include "EnginePch.h"
#define VMA_IMPLEMENTATION
#define VMA_DEBUG_LOG
#include <vma/vk_mem_alloc.h>

#include "VulkanAllocator.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"
#include "VulkanContext.h"

VulkanAllocator::VulkanAllocator(vk::Instance inInstance, const VulkanPhysicalDevice& inPhysicalDevice, const VulkanDevice& inDevice)
{
	myInstance = this;

	VmaAllocatorCreateInfo createInfo{};
	createInfo.vulkanApiVersion = VK_API_VERSION_1_3;
	createInfo.instance = inInstance;
	createInfo.physicalDevice = inPhysicalDevice.GetPhysicalDevice();
	createInfo.device = inDevice.GetDevice();

	vmaCreateAllocator(&createInfo, &myAllocator);
}

VulkanAllocator::~VulkanAllocator()
{
	VulkanContext::GetDevice()->waitIdle();
	for(int i = 0; i < VulkanContext::FrameLag; ++i)
	{
		TickBufferDeletes();
		TickImageDeletes();
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
}

VulkanBuffer* VulkanAllocator::AllocateBuffer_TS(const std::string& inName, const vk::BufferCreateInfo& inCreateInfo, VmaMemoryUsage inUsage, bool inMappable)
{
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
	outBuffer->mySize = inCreateInfo.size;

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
	myInstance->myBufferDeleteData.Add({ VulkanContext::FrameLag, inBuffer });
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
	myInstance->myImageDeleteData.Add({ VulkanContext::FrameLag, inImage });
}

VmaAllocator VulkanAllocator::GetVMAAllocator()
{
	return myInstance->myAllocator;
}

void VulkanAllocator::DestroyBufferInternal(VulkanBuffer* inBuffer)
{
#if DEBUG
	myInstance->myAllocatedNames.Remove(inBuffer->myName);
#endif
	vmaDestroyBuffer(myInstance->myAllocator, inBuffer->myBuffer, inBuffer->myAllocation);
	del(inBuffer);
}

void VulkanAllocator::DestroyImageInternal(VulkanImage* inImage)
{
#if DEBUG
	myInstance->myAllocatedNames.Remove(inImage->myName);
#endif
	vmaDestroyImage(myInstance->myAllocator, inImage->myImage, inImage->myAllocation);
	del(inImage);
}

void VulkanAllocator::TickBufferDeletes()
{
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
}

void VulkanAllocator::TickImageDeletes()
{
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
