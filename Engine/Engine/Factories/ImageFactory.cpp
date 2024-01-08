#include "EnginePch.h"
#include "ImageFactory.h"
#include "Serialization/BinaryWriter.h"
#include "Serialization/BinaryReader.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanDevice.h"

#include <tracy/Tracy.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Vulkan/VulkanImage.h"

VulkanImage* ImageFactory::GetImage(const std::filesystem::path& inPath)
{
	ZoneScoped;
	std::filesystem::path cachePath = GetCachePath(inPath);
	ImageData imageData;
	if(!std::filesystem::exists(cachePath))
	{
		LOG_WARNING("ImageFactory: Recaching %s", inPath.string().c_str());
		GetImageDataFromImageFile(inPath, imageData);
		SaveImageDataToBinary(imageData, cachePath);
		return CreateImageFromImageData(imageData);
	}

	GetImageDataFromBinary(cachePath, imageData);
	return CreateImageFromImageData(imageData);
}

VulkanImage* ImageFactory::CreateImageFromImageData(const ImageData& inImageData)
{
	ZoneScoped;
	VulkanImage* image;
	vk::BufferCreateInfo stagingCreateInfo = vk::BufferCreateInfo()
		.setSize(sizeof(size_t) * inImageData.myWidth * inImageData.myHeight)
		.setUsage(vk::BufferUsageFlagBits::eTransferSrc)
		.setSharingMode(vk::SharingMode::eExclusive);
	VulkanBuffer* stagingBuffer = VulkanAllocator::AllocateBuffer_TS("Texture staging buffer", stagingCreateInfo, VMA_MEMORY_USAGE_AUTO, true);

	void* ptr = stagingBuffer->Map();
	//check(inImageData.myChannels == 4); // Make sure channels is 4. We dont support anything else atm.
	memcpy(ptr, inImageData.myPixelData.data(), inImageData.myWidth * inImageData.myHeight * 4);
	stagingBuffer->Unmap();

	vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
		.setImageType(vk::ImageType::e2D)
		.setFormat(vk::Format::eR8G8B8A8Srgb)
		.setExtent({ static_cast<uint32_t>(inImageData.myWidth), static_cast<uint32_t>(inImageData.myHeight), 1 })
		.setMipLevels(1)
		.setArrayLayers(1)
		.setSamples(vk::SampleCountFlagBits::e1)
		.setTiling(vk::ImageTiling::eOptimal)
		.setUsage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
		.setSharingMode(vk::SharingMode::eExclusive)
		.setInitialLayout(vk::ImageLayout::eUndefined);

	std::string imageName = "VulkanImage - " + inImageData.mySourceFile.string();
	image = VulkanAllocator::AllocateImage_TS(imageName, createInfo, VMA_MEMORY_USAGE_AUTO);
#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
														   .setObjectHandle(VulkanContext::GetVulkanHandle(image->GetAPIResource()))
														   .setPObjectName(inImageData.mySourceFile.string().c_str())
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
		.setImage(image->GetAPIResource())
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

	commandBuffer.copyBufferToImage(stagingBuffer->GetAPIResource(), image->GetAPIResource(), vk::ImageLayout::eTransferDstOptimal, { bufferCopyRegion });

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlagBits(), {}, {},
								  vk::ImageMemoryBarrier()
								  .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
								  .setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eInputAttachmentRead)
								  .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
								  .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
								  .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
								  .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
								  .setImage(image->GetAPIResource())
								  .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)));

	VulkanContext::GetDevice().FlushCommandBuffer(commandBuffer);
	VulkanAllocator::DestroyBuffer_TS(stagingBuffer);

	image->CreateView(vk::ImageViewType::e2D);
#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
														   .setObjectHandle(VulkanContext::GetVulkanHandle(image->GetImageView()))
														   .setPObjectName(inImageData.mySourceFile.string().c_str())
														   .setObjectType(vk::ObjectType::eImageView));
#endif
	return image;
}

void ImageFactory::GetImageDataFromImageFile(const std::filesystem::path& inPath, ImageData& out)
{
	ZoneScoped;
	check(std::filesystem::exists(inPath));
	out.mySourceFile = inPath;

	stbi_uc* pixels = stbi_load(inPath.string().c_str(), &out.myWidth, &out.myHeight, &out.myChannels, STBI_rgb_alpha);
	check(pixels);
	out.myPixelData.Resize(out.myWidth * out.myHeight * 4);
	memcpy(out.myPixelData.data(), pixels, out.myPixelData.size());
	stbi_image_free(pixels);
}

void ImageFactory::GetImageDataFromBinary(const std::filesystem::path& inPath, ImageData& out)
{
	ZoneScoped;
	BinaryReader reader(inPath);
	int binaryFileVersion;
	reader.Read(binaryFileVersion);
	check(binaryFileVersion == FileVersion);

	reader.Read(out.mySourceFile);

	const std::filesystem::file_time_type sourceFileLastWriteTime = std::filesystem::last_write_time(out.mySourceFile);
	std::filesystem::file_time_type cachedWriteTime;
	reader.Read(cachedWriteTime);
	if (cachedWriteTime != sourceFileLastWriteTime)
	{
		check(false);
		return;
	}

	reader.Read(out.myWidth);
	reader.Read(out.myHeight);
	reader.Read(out.myChannels);
	reader.Read(out.myPixelData);
}

void ImageFactory::SaveImageDataToBinary(const ImageData& inData, const std::filesystem::path& inSavePath)
{
	ZoneScoped;
	BinaryWriter writer(inSavePath);
	writer.Write(FileVersion);
	writer.Write(inData.mySourceFile);

	const std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(inData.mySourceFile);
	writer.Write(lastWriteTime);

	writer.Write(inData.myWidth);
	writer.Write(inData.myHeight);
	writer.Write(inData.myChannels);
	writer.Write(inData.myPixelData);
	writer.Save();
}

std::filesystem::path ImageFactory::GetCachePath(const std::filesystem::path& inFilePath)
{
	std::filesystem::path cachePath = L"Cache/ImageCache/" + inFilePath.filename().wstring() + L".image";
	return cachePath;
}
