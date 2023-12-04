#include "EnginePch.h"
#define VMA_IMPLEMENTATION
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
#endif

	return outBuffer;
}

void VulkanAllocator::DestroyBuffer(VulkanBuffer* inBuffer)
{
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
#endif

	return outImage;
}

void VulkanAllocator::DestroyImage(VulkanImage* inImage)
{
	vmaDestroyImage(myAllocator, inImage->myImage, inImage->myAllocation);
	del(inImage);
}
