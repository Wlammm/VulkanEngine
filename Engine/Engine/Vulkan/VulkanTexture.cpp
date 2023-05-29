#include "EnginePch.h"
#include "VulkanTexture.h"
#include "VulkanContext.h"
#include "VulkanAllocator.h"
#include "VulkanImage.h"
#include "VulkanDevice.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "VulkanBuffer.h"

VulkanTexture::VulkanTexture(const std::filesystem::path& inPath, SamplerMode inSamplerMode)
{
	CreateImage(inPath);
	CreateSampler(inSamplerMode);
}

VulkanTexture::VulkanTexture(VulkanImage* inImage, SamplerMode inSamplerMode)
{
	myImage = inImage;
	CreateSampler(inSamplerMode);

	vk::ImageSubresourceRange subresourceRange = vk::ImageSubresourceRange()
		.setAspectMask(vk::ImageAspectFlagBits::eColor)
		.setBaseMipLevel(0)
		.setLevelCount(1)
		.setLayerCount(1);
	myImage->CreateView(vk::ImageViewType::e2D, subresourceRange);
}

VulkanTexture::~VulkanTexture()
{
	LOG_WARNING("Textures waits for device idle. Fix");
	VulkanContext::GetDevice()->waitIdle();

	VulkanContext::GetAllocator().DestroyImage(myImage);
	VulkanContext::GetDevice()->destroySampler(mySampler);
}

vk::Sampler VulkanTexture::GetSampler() const
{
	return mySampler;
}

vk::ImageView VulkanTexture::GetImageView() const
{
	return myImage->GetImageView();
}

VulkanImage* VulkanTexture::GetImage() const
{
	return myImage;
}

void VulkanTexture::CreateImage(const std::filesystem::path& inPath)
{
	int width, height, channels;
	stbi_uc* pixels = stbi_load(inPath.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);
	check(pixels && "Failed to load texture.");

	vk::BufferCreateInfo stagingCreateInfo = vk::BufferCreateInfo()
		.setSize(sizeof(size_t) * width * height)
		.setUsage(vk::BufferUsageFlagBits::eTransferSrc)
		.setSharingMode(vk::SharingMode::eExclusive);
	VulkanBuffer* stagingBuffer = VulkanContext::GetAllocator().AllocateBuffer("Texture staging buffer", stagingCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU);

	void* ptr = stagingBuffer->Map();
	memcpy(ptr, pixels, width * height * 4);
	stagingBuffer->Unmap();
	stbi_image_free(pixels);

	vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
		.setImageType(vk::ImageType::e2D)
		.setFormat(vk::Format::eR8G8B8A8Srgb)
		.setExtent({ static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 })
		.setMipLevels(1)
		.setArrayLayers(1)
		.setSamples(vk::SampleCountFlagBits::e1)
		.setTiling(vk::ImageTiling::eOptimal)
		.setUsage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
		.setSharingMode(vk::SharingMode::eExclusive)
		.setInitialLayout(vk::ImageLayout::eUndefined);

	myImage = VulkanContext::GetAllocator().AllocateImage("Vulkan texture", createInfo, VMA_MEMORY_USAGE_GPU_ONLY);

	vk::CommandBuffer commandBuffer = VulkanContext::GetDevice().CreateCommandBuffer(true);

	vk::ImageSubresourceRange subresourceRange = vk::ImageSubresourceRange()
		.setAspectMask(vk::ImageAspectFlagBits::eColor)
		.setBaseMipLevel(0)
		.setLevelCount(1)
		.setLayerCount(1);

	vk::ImageMemoryBarrier imageMemoryBarrier = vk::ImageMemoryBarrier()
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setImage(*myImage)
		.setSubresourceRange(subresourceRange)
		.setSrcAccessMask(vk::AccessFlagBits::eNone)
		.setDstAccessMask(vk::AccessFlagBits::eTransferWrite)
		.setOldLayout(vk::ImageLayout::eUndefined)
		.setNewLayout(vk::ImageLayout::eTransferDstOptimal);

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eHost, vk::PipelineStageFlagBits::eTransfer, vk::DependencyFlagBits(), {}, {}, { imageMemoryBarrier });

	vk::BufferImageCopy bufferCopyRegion{};
	bufferCopyRegion.imageSubresource
		.setAspectMask(vk::ImageAspectFlagBits::eColor)
		.setMipLevel(0)
		.setBaseArrayLayer(0)
		.setLayerCount(1);
	bufferCopyRegion.imageExtent
		.setWidth(width)
		.setHeight(height)
		.setDepth(1);
	bufferCopyRegion.setBufferOffset(0);

	commandBuffer.copyBufferToImage(*stagingBuffer, *myImage, vk::ImageLayout::eTransferDstOptimal, { bufferCopyRegion });

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlagBits(), {}, {},
		vk::ImageMemoryBarrier()
		.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
		.setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eInputAttachmentRead)
		.setOldLayout(vk::ImageLayout::eTransferDstOptimal)
		.setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setImage(*myImage)
		.setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)));

	VulkanContext::GetDevice().FlushCommandBuffer(commandBuffer);
	VulkanContext::GetAllocator().DestroyBuffer(stagingBuffer);

	myImage->CreateView(vk::ImageViewType::e2D, subresourceRange);
}

void VulkanTexture::CreateSampler(SamplerMode inSamplerMode)
{
	mySampler = VulkanUtils::CreateSampler(inSamplerMode);
}
