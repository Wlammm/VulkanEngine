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
	mySubresourceRange = vk::ImageSubresourceRange()
		.setAspectMask(vk::ImageAspectFlagBits::eColor)
		.setBaseMipLevel(0)
		.setLevelCount(myNumMipLevels)
		.setLayerCount(1);
	CreateView(inViewType, mySubresourceRange);

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
	mySubresourceRange = inRange;
	
	vk::ImageViewCreateInfo createInfo = vk::ImageViewCreateInfo()
		.setImage(myImage)
		.setViewType(inViewType)
		.setFormat(myFormat)
		.setSubresourceRange(inRange);

	myView = VulkanContext::GetDevice()->createImageView(createInfo);
}

void VulkanImage::CreateDepthView()
{
	mySubresourceRange = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1);
	
	CreateView(vk::ImageViewType::e2D, mySubresourceRange);
}

const glm::vec2& VulkanImage::GetSize() const
{
	return mySize;
}

uint VulkanImage::GetNumMipLevels() const
{
	return myNumMipLevels;
}

vk::SampleCountFlagBits VulkanImage::GetNumMSAASamples() const
{
	return myNumMSAASamples;
}

vk::ImageSubresourceRange VulkanImage::GetSubresourceRange() const
{
	return mySubresourceRange;
}

VulkanImage* VulkanImage::CreateFromSwapchainImage(vk::Image inImage, vk::Format inFormat)
{
	VulkanImage* image = new VulkanImage();
	image->myImage = inImage;
	image->myFormat = inFormat;
	return image;
}

void VulkanImage::DestroySwapchainImage(VulkanImage* inImage)
{
	del(inImage);
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

bool VulkanImage::HasImageView() const
{
	return myView != nullptr;
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