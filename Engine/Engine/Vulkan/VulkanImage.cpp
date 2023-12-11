#include "EnginePch.h"
#include "VulkanImage.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanAllocator.h"
#include "VulkanBuffer.h"
#include <tracy/Tracy.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

VulkanImage::~VulkanImage()
{
	if (myView)
	{
		VulkanContext::GetDevice()->destroyImageView(myView);
		myView = nullptr;
	}

	if(mySampler)
	{
		VulkanContext::GetDevice()->destroySampler(mySampler);
		mySampler = nullptr;
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

VulkanImage::operator vk::Image()
{
	return myImage;
}

vk::Image VulkanImage::operator->()
{
	return myImage;
}

VulkanImage* VulkanImage::LoadFromFile(const std::filesystem::path& inPath)
{
	ZoneScoped;
	
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

vk::Sampler VulkanImage::GetSampler() const
{
	check(mySampler);
	return mySampler;
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

void VulkanImage::CreateSampler(SamplerMode inSamplerMode)
{
	ZoneScoped;
	mySampler = VulkanUtils::CreateSampler(inSamplerMode);
}
