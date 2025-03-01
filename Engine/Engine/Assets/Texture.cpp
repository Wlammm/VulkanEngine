#include "EnginePch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Rendering/RenderSystem.h"
#include "Rendering/TextureSystem.h"
#include "Serialization/BinaryReader.h"
#include "Utils/ThreadUtils.hpp"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanImage.h"
#include "Vulkan/VulkanPhysicalDevice.h"

const vk::Format IMAGE_FORMAT = vk::Format::eR8G8B8A8Unorm;

Coroutine<void, void, false> Texture::Load(const std::filesystem::path inPath)
{
    ImageData imageData{};

    bool loadFromSourceFile = true;
    if(IsCached(inPath))
    {
        bool success = TryLoadImageDataFromCache(GetCachePath(inPath), imageData);
        loadFromSourceFile = !success;
    }

    if(loadFromSourceFile)
    {
        imageData = LoadImageDataFromImageFile(inPath);
    }

    InitializeFromImageData(imageData);

	co_return;
}

void Texture::Unload()
{
	VulkanAllocator::DestroyImage_TS(myImage);
	myImage = nullptr;
}

VulkanImage* Texture::GetImage() const
{
	return myImage;
}
vk::ImageView Texture::GetImageView() const
{
	return myImage->GetImageView();
}

uint Texture::GetBindlessIndex() const
{
	return myBindlessIndex;
}

bool Texture::IsCached(const std::filesystem::path& inPath)
{
    return std::filesystem::exists(GetCachePath(inPath));    
}

CachePath Texture::GetCachePath(const std::filesystem::path& inFilePath)
{
    return L"Cache/ImageCache/" + inFilePath.filename().wstring() + L".image";
}

ImageData Texture::LoadImageDataFromImageFile(const std::filesystem::path& inPath)
{
    ZoneScoped;
    check(std::filesystem::exists(inPath));
    
    ImageData imageData{};
    imageData.mySourceFile = inPath;

    stbi_uc* pixels = stbi_load(inPath.string().c_str(), &imageData.myWidth, &imageData.myHeight, &imageData.myChannels, STBI_rgb_alpha);
    check(pixels);
	check(imageData.myWidth != 0 && imageData.myHeight != 0);

	imageData.myNumMipLevels = static_cast<uint>(std::floor(std::log2(std::max(imageData.myHeight, imageData.myWidth)))) + 1;
	
    imageData.myPixelData.Resize(imageData.myWidth * imageData.myHeight * 4);
    memcpy(imageData.myPixelData.data(), pixels, imageData.myPixelData.size());
    stbi_image_free(pixels);
    return imageData;
}

bool Texture::TryLoadImageDataFromCache(const CachePath& inPath, ImageData& out)
{
	ZoneScoped;
	BinaryReader reader(inPath);
	int binaryFileVersion;
	reader.Read(binaryFileVersion);

	if(binaryFileVersion != FileVersion)
	{
		LOG_WARNING("Image format is outdated. Recaching %s", inPath.string().c_str());
		return false;
	}
	
	check(binaryFileVersion == FileVersion);

	reader.Read(out.mySourceFile);

	const std::filesystem::file_time_type sourceFileLastWriteTime = std::filesystem::last_write_time(out.mySourceFile);
	std::filesystem::file_time_type cachedWriteTime;
	reader.Read(cachedWriteTime);
	if (cachedWriteTime != sourceFileLastWriteTime)
	{
		LOG_WARNING("Image cache is outdated. Recaching %s", inPath.string().c_str());
		return false;
	}

	reader.Read(out.myWidth);
	reader.Read(out.myHeight);
	reader.Read(out.myChannels);
	reader.Read(out.myNumMipLevels);
	reader.Read(out.myPixelData);
	return true;
}

void Texture::InitializeFromImageData(const ImageData& inImageData)
{
    ZoneScoped;
	vk::BufferCreateInfo stagingCreateInfo = vk::BufferCreateInfo()
		.setSize(sizeof(size_t) * inImageData.myWidth * inImageData.myHeight)
		.setUsage(vk::BufferUsageFlagBits::eTransferSrc)
		.setSharingMode(vk::SharingMode::eExclusive);
	VulkanBuffer* stagingBuffer = VulkanAllocator::AllocateBuffer_TS("Texture staging buffer", stagingCreateInfo, VMA_MEMORY_USAGE_AUTO, true);

	void* ptr = stagingBuffer->GetPtr();
	//check(inImageData.myChannels == 4); // Make sure channels is 4. We dont support anything else atm.
	memcpy(ptr, inImageData.myPixelData.data(), inImageData.myWidth * inImageData.myHeight * 4);

	vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
		.setImageType(vk::ImageType::e2D)
		.setFormat(IMAGE_FORMAT)
		.setExtent({ static_cast<uint32_t>(inImageData.myWidth), static_cast<uint32_t>(inImageData.myHeight), 1 })
		.setMipLevels(inImageData.myNumMipLevels)
		.setArrayLayers(1)
		.setSamples(vk::SampleCountFlagBits::e1)
		.setTiling(vk::ImageTiling::eOptimal)
		.setUsage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eSampled)
		.setSharingMode(vk::SharingMode::eExclusive)
		.setInitialLayout(vk::ImageLayout::eUndefined);

	std::string imageName = "VulkanImage - " + inImageData.mySourceFile.string();
	myImage = VulkanAllocator::AllocateImage_TS(imageName, createInfo, VMA_MEMORY_USAGE_AUTO);
	
#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
														   .setObjectHandle(VulkanContext::GetVulkanHandle(myImage->GetAPIResource()))
														   .setPObjectName(inImageData.mySourceFile.string().c_str())
														   .setObjectType(vk::ObjectType::eImage));
#endif
	myImage->CreateView(vk::ImageViewType::e2D);

	vk::CommandBuffer commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();

	vk::ImageSubresourceRange subresourceRange = vk::ImageSubresourceRange()
	.setAspectMask(vk::ImageAspectFlagBits::eColor)
	.setBaseMipLevel(0)
	.setLevelCount(inImageData.myNumMipLevels)
	.setLayerCount(1);

	vk::ImageMemoryBarrier imageMemoryBarrier = vk::ImageMemoryBarrier()
		.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
		.setImage(myImage->GetAPIResource())
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
		.setWidth(inImageData.myWidth)
		.setHeight(inImageData.myHeight)
		.setDepth(1);
	bufferCopyRegion.setBufferOffset(0);
	
	commandBuffer.copyBufferToImage(stagingBuffer->GetAPIResource(), myImage->GetAPIResource(), vk::ImageLayout::eTransferDstOptimal, { bufferCopyRegion });

	// Not needed as generate mip levels will transition it to correct layout.
	//inCommandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlagBits(), {}, {},
	//							  vk::ImageMemoryBarrier()
	//							  .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
	//							  .setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eInputAttachmentRead)
	//							  .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
	//							  .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
	//							  .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
	//							  .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
	//							  .setImage(myImage->GetAPIResource())
	//							  .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)));
	
	
	GenerateMipLevels(commandBuffer);
	RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer);
	VulkanAllocator::DestroyBuffer_TS(stagingBuffer);
#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
														   .setObjectHandle(VulkanContext::GetVulkanHandle(myImage->GetImageView()))
														   .setPObjectName(inImageData.mySourceFile.string().c_str())
														   .setObjectType(vk::ObjectType::eImageView));
#endif
	TextureSystem::RegisterTexture_TS(this);
}

void Texture::GenerateMipLevels(vk::CommandBuffer inCommandBuffer)
{
	uint mipLevels = myImage->GetNumMipLevels();
	
	vk::FormatProperties formatProperties = VulkanContext::GetPhysicalDevice()->getFormatProperties(IMAGE_FORMAT);

	// TODO: Disable mip maps if not supported.
	check(formatProperties.optimalTilingFeatures& vk::FormatFeatureFlagBits::eSampledImageFilterLinear);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = myImage->GetAPIResource();
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = static_cast<int32_t>(myImage->GetSize().x);
	int32_t mipHeight = static_cast<int32_t>(myImage->GetSize().y);
	
	for(uint i = 1; i < myImage->GetNumMipLevels(); ++i)
	{
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(inCommandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

		VkImageBlit blit{};
		blit.srcOffsets[0] = {0, 0, 0};
		blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = {0, 0, 0};
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(inCommandBuffer,
			myImage->GetAPIResource(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			myImage->GetAPIResource(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(inCommandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(inCommandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier);
}
