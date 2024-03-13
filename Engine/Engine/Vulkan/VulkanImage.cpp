#include "EnginePch.h"
#include "VulkanImage.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanAllocator.h"
#include "VulkanBuffer.h"
#include <tracy/Tracy.hpp>

VulkanImage::~VulkanImage()
{
	if (myView)
	{
		VulkanContext::GetDevice()->destroyImageView(myView);
		myView = nullptr;
	}
}

void VulkanImage::CreateView(vk::ImageViewType inViewType)
{
	vk::ImageSubresourceRange subresourceRange = vk::ImageSubresourceRange()
		.setAspectMask(vk::ImageAspectFlagBits::eColor)
		.setBaseMipLevel(0)
		.setLevelCount(1)
		.setLayerCount(1);
	CreateView(inViewType, subresourceRange);

#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
		.setObjectHandle(VulkanContext::GetVulkanHandle(myView))
		.setPObjectName(myName.c_str())
		.setObjectType(vk::ObjectType::eImageView));
#endif
}

void VulkanImage::CreateView(vk::ImageViewType inViewType, vk::ImageSubresourceRange inRange)
{
	ZoneScoped;
	check(!myView);

	vk::ImageViewCreateInfo createInfo = vk::ImageViewCreateInfo()
		.setImage(myImage)
		.setViewType(inViewType)
		.setFormat(myFormat)
		.setSubresourceRange(inRange);

	myView = VulkanContext::GetDevice()->createImageView(createInfo);
}

void VulkanImage::CreateDepthView()
{
	CreateView(vk::ImageViewType::e2D, vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1));
}

const glm::vec2& VulkanImage::GetSize() const
{
	return mySize;
}

vk::Image VulkanImage::GetAPIResource() const
{
	return myImage;
}

vk::ImageView VulkanImage::GetImageView() const
{
	check(myView);
	return myView;
}

vk::Format VulkanImage::GetFormat() const
{
	return myFormat;
}

void* VulkanImage::Map()
{
	void* ptr;
	vmaMapMemory(VulkanAllocator::GetVMAAllocator(), myAllocation, &ptr);
	return ptr;
}

void VulkanImage::Unmap()
{
	vmaUnmapMemory(VulkanAllocator::GetVMAAllocator(), myAllocation);
}