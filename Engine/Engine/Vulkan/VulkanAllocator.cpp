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
#if DEBUG

	myInstance->myAllocatedNamesMutex.lock();
	for(const std::string& allocationName : myAllocatedNames)
	{
		LOG_ERROR("Unallocated buffer: %s", allocationName.c_str());
	}
	myInstance->myAllocatedNamesMutex.unlock();
#endif

	vmaDestroyAllocator(myAllocator);
	myInstance = nullptr;
}

VulkanBuffer* VulkanAllocator::AllocateBuffer_TS(const std::string& inName, const vk::BufferCreateInfo& inCreateInfo, VmaMemoryUsage inUsage)
{
	VulkanBuffer* outBuffer = new VulkanBuffer();
	VmaAllocationCreateInfo allocCreateInfo{};
	allocCreateInfo.usage = inUsage;
	VkBuffer buffer;
	VkBufferCreateInfo info = inCreateInfo;
	vmaCreateBuffer(myInstance->myAllocator, &info, &allocCreateInfo, &buffer, &outBuffer->myAllocation, nullptr);
	outBuffer->myBuffer = buffer;

#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
		.setObjectHandle(VulkanContext::GetVulkanHandle(outBuffer->GetAPIResource()))
		.setPObjectName(inName.c_str())
		.setObjectType(vk::ObjectType::eBuffer));
	outBuffer->myName = inName;
	myInstance->myAllocatedNamesMutex.lock();
	myInstance->myAllocatedNames.Add(inName);
	myInstance->myAllocatedNamesMutex.unlock();
#endif

	return outBuffer;
}

void VulkanAllocator::DestroyBuffer_TS(VulkanBuffer* inBuffer)
{
#if DEBUG
	myInstance->myAllocatedNamesMutex.lock();
	myInstance->myAllocatedNames.Remove(inBuffer->myName);
	myInstance->myAllocatedNamesMutex.unlock();
#endif
	vmaDestroyBuffer(myInstance->myAllocator, inBuffer->myBuffer, inBuffer->myAllocation);
	del(inBuffer);
}

VulkanImage* VulkanAllocator::AllocateImage_TS(const std::string& inName, const vk::ImageCreateInfo& inCreateInfo, VmaMemoryUsage inUsage)
{
	VulkanImage* outImage = new VulkanImage();
	VmaAllocationCreateInfo allocCreateInfo{};
	allocCreateInfo.usage = inUsage;

	VkImage image;
	const VkImageCreateInfo info = inCreateInfo;
	vmaCreateImage(myInstance->myAllocator, &info, &allocCreateInfo, &image, &outImage->myAllocation, nullptr);
	outImage->myImage = image;
	outImage->myFormat = inCreateInfo.format;

#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
		.setObjectHandle(VulkanContext::GetVulkanHandle(outImage->GetAPIResource()))
		.setPObjectName(inName.c_str())
		.setObjectType(vk::ObjectType::eImage));
	outImage->myName = inName;
	myInstance->myAllocatedNamesMutex.lock();
	myInstance->myAllocatedNames.Add(inName);
	myInstance->myAllocatedNamesMutex.unlock();
#endif

	return outImage;
}

void VulkanAllocator::DestroyImage_TS(VulkanImage* inImage)
{
#if DEBUG
	myInstance->myAllocatedNamesMutex.lock();
	myInstance->myAllocatedNames.Remove(inImage->myName);
	myInstance->myAllocatedNamesMutex.unlock();
#endif
	vmaDestroyImage(myInstance->myAllocator, inImage->myImage, inImage->myAllocation);
	del(inImage);
}

VmaAllocator VulkanAllocator::GetVMAAllocator()
{
	return myInstance->myAllocator;
}
