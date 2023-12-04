#include "EnginePch.h"
#include "VulkanImage.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanAllocator.h"
#include "VulkanBuffer.h"

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
	check(std::filesystem::exists(inPath));

	VulkanImage* image;
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

	image = VulkanContext::GetAllocator().AllocateImage("Vulkan texture", createInfo, VMA_MEMORY_USAGE_GPU_ONLY);
#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
		.setObjectHandle(VulkanContext::GetVulkanHandle(image->operator vk::Image()))
		.setPObjectName(inPath.string().c_str())
		.setObjectType(vk::ObjectType::eImage));
#endif

	vk::CommandBuffer commandBuffer = VulkanContext::GetDevice().CreateCommandBuffer(true);

	vk::ImageSubresourceRange subresourceRange = vk::ImageSubresourceRange()
		.setAspectMask(vk::ImageAspectFlagBits::eColor)
		.setBaseMipLevel(0)
		.setLevelCount(1)
		.setLayerCount(1);

	vk::ImageMemoryBarrier imageMemoryBarrier = vk::ImageMemoryBarrier()
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setImage(*image)
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

	commandBuffer.copyBufferToImage(*stagingBuffer, *image, vk::ImageLayout::eTransferDstOptimal, { bufferCopyRegion });

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlagBits(), {}, {},
		vk::ImageMemoryBarrier()
		.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
		.setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eInputAttachmentRead)
		.setOldLayout(vk::ImageLayout::eTransferDstOptimal)
		.setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setImage(*image)
		.setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)));

	VulkanContext::GetDevice().FlushCommandBuffer(commandBuffer);
	VulkanContext::GetAllocator().DestroyBuffer(stagingBuffer);

	image->CreateView(vk::ImageViewType::e2D);
#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
		.setObjectHandle(VulkanContext::GetVulkanHandle(image->GetImageView()))
		.setPObjectName(inPath.string().c_str())
		.setObjectType(vk::ObjectType::eImageView));
#endif

	return image;
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
	mySampler = VulkanUtils::CreateSampler(inSamplerMode);
}
