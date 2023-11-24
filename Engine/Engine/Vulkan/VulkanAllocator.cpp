#include "EnginePch.h"
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

#include "VulkanAllocator.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"

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
	for(const auto& buffer : myBuffers)
	{
		std::string s = "GPU buffer allocated but never freed: " + buffer->myName;
		LOG_ERROR(s);
	}

	for (const auto& image : myImages)
	{
		std::string s = "GPU image allocated but never freed: " + image->myName;
		LOG_ERROR(s);
	}
#endif

	vmaDestroyAllocator(myAllocator);
}

VulkanBuffer* VulkanAllocator::AllocateBuffer(const std::string& inName, const vk::BufferCreateInfo& inCreateInfo, VmaMemoryUsage inUsage)
{
	VulkanBuffer* outBuffer = new VulkanBuffer();
#ifdef DEBUG
	outBuffer->myName = inName;
	myBuffers.Add(outBuffer);
#endif

	VmaAllocationCreateInfo allocCreateInfo{};
	allocCreateInfo.usage = inUsage;

	VkBuffer buffer;
	VkBufferCreateInfo info = inCreateInfo;
	vmaCreateBuffer(myAllocator, &info, &allocCreateInfo, &buffer, &outBuffer->myAllocation, nullptr);
	outBuffer->myBuffer = buffer;
	return outBuffer;
}

void VulkanAllocator::DestroyBuffer(VulkanBuffer* inBuffer)
{
	vmaDestroyBuffer(myAllocator, inBuffer->myBuffer, inBuffer->myAllocation);

#ifdef DEBUG
	myBuffers.Remove(inBuffer);
#endif
	del(inBuffer);
}

VulkanImage* VulkanAllocator::AllocateImage(const std::string& inName, const vk::ImageCreateInfo& inCreateInfo, VmaMemoryUsage inUsage)
{
	VulkanImage* outImage = new VulkanImage();
#ifdef DEBUG
	outImage->myName = inName;
	myImages.Add(outImage);
#endif

	VmaAllocationCreateInfo allocCreateInfo{};
	allocCreateInfo.usage = inUsage;

	VkImage image;
	const VkImageCreateInfo info = inCreateInfo;
	vmaCreateImage(myAllocator, &info, &allocCreateInfo, &image, &outImage->myAllocation, nullptr);
	outImage->myImage = image;
	outImage->myFormat = inCreateInfo.format;
	return outImage;
}

void VulkanAllocator::DestroyImage(VulkanImage* inImage)
{
	vmaDestroyImage(myAllocator, inImage->myImage, inImage->myAllocation);

#ifdef DEBUG
	myImages.Remove(inImage);
#endif
	del(inImage);
}
