#include "EnginePch.h"
#include "VulkanRenderImages.h"
#include "VulkanImage.h"

VulkanRenderImages::VulkanRenderImages(const vk::ImageCreateInfo& inCreateInfo)
{


	//vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
	//	.setMipLevels(1)
	//	.setArrayLayers(1)
	//	.setFormat(vk::Format::eR32G32B32Sfloat)
	//	.setImageType(vk::ImageType::e2D)
	//	.setInitialLayout(vk::ImageLayout::eUndefined)
	//	.setExtent({ Engine::GetRenderResolution().x, Engine::GetRenderResolution().y, 1 });
	//myRenderTexture = VulkanContext::GetAllocator().AllocateImage("Render texture", createInfo, VMA_MEMORY_USAGE_GPU_ONLY);
	//myRenderTexture->CreateView(vk::ImageViewType::e2D, vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
}

vk::ImageView VulkanRenderImages::GetView(const uint inIndex)
{
	return myImages[inIndex]->GetImageView();
}
