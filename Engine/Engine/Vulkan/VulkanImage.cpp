#include "EnginePch.h"
#include "VulkanImage.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanAllocator.h"

VulkanImage::~VulkanImage()
{
	if (myView)
	{
		VulkanContext::GetDevice()->destroyImageView(myView);
		myView = nullptr;
	}
}

void VulkanImage::CreateView(vk::ImageViewType inViewType, vk::ImageSubresourceRange inSubResourceRange)
{
	check(!myView);

	auto createInfo = vk::ImageViewCreateInfo()
		.setImage(myImage)
		.setViewType(inViewType)
		.setFormat(myFormat)
		.setSubresourceRange(inSubResourceRange);

	myView = VulkanContext::GetDevice()->createImageView(createInfo);
}

VulkanImage::operator vk::Image()
{
	return myImage;
}

vk::Image VulkanImage::operator->()
{
	return myImage;
}

vk::ImageView VulkanImage::GetImageView() const
{
	return myView;
}

vk::Format VulkanImage::GetFormat()
{
	return myFormat;
}

void* VulkanImage::Map()
{
	void* ptr;
	vmaMapMemory(VulkanContext::GetAllocator(), myAllocation, &ptr);
	return ptr;
}

void VulkanImage::Unmap()
{
	vmaUnmapMemory(VulkanContext::GetAllocator(), myAllocation);
}
