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
	for(const std::string& allocationName : myAllocatedNames)
	{
		LOG_ERROR("Unallocated buffer: %s", allocationName.c_str());
	}
#endif

	vmaDestroyAllocator(myAllocator);
}

VulkanBuffer* VulkanAllocator::AllocateBuffer(const std::string& inName, const vk::BufferCreateInfo& inCreateInfo, VmaMemoryUsage inUsage)
{
	VulkanBuffer* outBuffer = new VulkanBuffer();
	VmaAllocationCreateInfo allocCreateInfo{};
	allocCreateInfo.usage = inUsage;
	VkBuffer buffer;
	VkBufferCreateInfo info = inCreateInfo;
	vmaCreateBuffer(myAllocator, &info, &allocCreateInfo, &buffer, &outBuffer->myAllocation, nullptr);
	outBuffer->myBuffer = buffer;

#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
		.setObjectHandle(VulkanContext::GetVulkanHandle(outBuffer->operator vk::Buffer()))
		.setPObjectName(inName.c_str())
		.setObjectType(vk::ObjectType::eBuffer));
	outBuffer->myName = inName;
	myAllocatedNames.Add(inName);
#endif

	return outBuffer;
}

void VulkanAllocator::DestroyBuffer(VulkanBuffer* inBuffer)
{
#if DEBUG
	myAllocatedNames.Remove(inBuffer->myName);
#endif
	vmaDestroyBuffer(myAllocator, inBuffer->myBuffer, inBuffer->myAllocation);
	del(inBuffer);
}

VulkanImage* VulkanAllocator::AllocateImage(const std::string& inName, const vk::ImageCreateInfo& inCreateInfo, VmaMemoryUsage inUsage)
{
	VulkanImage* outImage = new VulkanImage();
	VmaAllocationCreateInfo allocCreateInfo{};
	allocCreateInfo.usage = inUsage;

	VkImage image;
	const VkImageCreateInfo info = inCreateInfo;
	vmaCreateImage(myAllocator, &info, &allocCreateInfo, &image, &outImage->myAllocation, nullptr);
	outImage->myImage = image;
	outImage->myFormat = inCreateInfo.format;

#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
		.setObjectHandle(VulkanContext::GetVulkanHandle(outImage->operator vk::Image()))
		.setPObjectName(inName.c_str())
		.setObjectType(vk::ObjectType::eImage));
	outImage->myName = inName;
	myAllocatedNames.Add(inName);
#endif

	return outImage;
}

void VulkanAllocator::DestroyImage(VulkanImage* inImage)
{
#if DEBUG
	myAllocatedNames.Remove(inImage->myName);
#endif
	vmaDestroyImage(myAllocator, inImage->myImage, inImage->myAllocation);
	del(inImage);
}
