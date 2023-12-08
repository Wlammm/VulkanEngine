#include "EnginePch.h"
#include "VulkanDepthBuffer.h"
#include "Engine.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanAllocator.h"
#include "VulkanImage.h"

VulkanDepthBuffer::VulkanDepthBuffer(const Vec2ui& inResolution)
{
	const auto createInfo = vk::ImageCreateInfo()
		.setImageType(vk::ImageType::e2D)
		.setFormat(vk::Format::eD32Sfloat)
		.setExtent({ inResolution.x, inResolution.y, 1 })
		.setMipLevels(1)
		.setArrayLayers(1)
		.setSamples(vk::SampleCountFlagBits::e1)
		.setTiling(vk::ImageTiling::eOptimal)
		.setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment)
		.setSharingMode(vk::SharingMode::eExclusive)
		.setInitialLayout(vk::ImageLayout::eUndefined);
	myImage = VulkanContext::GetAllocator().AllocateImage("Depth buffer", createInfo, VMA_MEMORY_USAGE_GPU_ONLY);
	myImage->CreateView(vk::ImageViewType::e2D, vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1));
}

VulkanDepthBuffer::~VulkanDepthBuffer()
{
	VulkanContext::GetAllocator().DestroyImage(myImage);
	myImage = nullptr;
}

vk::ImageView VulkanDepthBuffer::GetImageView() const
{
	return myImage->GetImageView();
}

vk::Format VulkanDepthBuffer::GetFormat() const
{
	return myImage->GetFormat();
}
