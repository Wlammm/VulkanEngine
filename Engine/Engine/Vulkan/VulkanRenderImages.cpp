#include "EnginePch.h"
#include "VulkanRenderImages.h"
#include "VulkanImage.h"
#include "VulkanContext.h"
#include "VulkanSwapChain.h"
#include "VulkanAllocator.h"

VulkanRenderImages::VulkanRenderImages(const vk::ImageCreateInfo& inCreateInfo)
{
	for (uint i = 0; i < VulkanContext::GetSwapChain().GetFrameLag(); ++i)
	{
		myImages.Add(VulkanContext::GetAllocator().AllocateImage("Render images", inCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY));
		myImages.Last()->CreateView(vk::ImageViewType::e2D, vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
	}
}

vk::ImageView VulkanRenderImages::GetView(const uint inIndex)
{
	return myImages[inIndex]->GetImageView();
}

vk::Format VulkanRenderImages::GetFormat() const
{
	// Since all images are the same, we can just query properties from the first one.
	return myImages.First()->GetFormat();
}
