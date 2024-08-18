#include "EnginePch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Rendering/RenderSystem.h"
#include "Serialization/BinaryReader.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanImage.h"

void Texture::Load(const std::filesystem::path& inPath)
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
        LoadImageDataFromImageFile(inPath);
    }

    InitializeFromImageData(imageData);
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
	myImage = VulkanAllocator::AllocateImage_TS(imageName, createInfo, VMA_MEMORY_USAGE_AUTO);
	
#if DEBUG
	VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
														   .setObjectHandle(VulkanContext::GetVulkanHandle(myImage->GetAPIResource()))
														   .setPObjectName(inImageData.mySourceFile.string().c_str())
														   .setObjectType(vk::ObjectType::eImage));
#endif

	RenderSystem::AddUploadCommand_TS(this, [this, inImageData, stagingBuffer](vk::CommandBuffer inCommandBuffer)
	{
		vk::ImageSubresourceRange subresourceRange = vk::ImageSubresourceRange()
		.setAspectMask(vk::ImageAspectFlagBits::eColor)
		.setBaseMipLevel(0)
		.setLevelCount(1)
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
		inCommandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eHost, vk::PipelineStageFlagBits::eTransfer, vk::DependencyFlagBits(), {}, {}, { imageMemoryBarrier });
		
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
		
		inCommandBuffer.copyBufferToImage(stagingBuffer->GetAPIResource(), myImage->GetAPIResource(), vk::ImageLayout::eTransferDstOptimal, { bufferCopyRegion });
		
		inCommandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlagBits(), {}, {},
									  vk::ImageMemoryBarrier()
									  .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
									  .setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eInputAttachmentRead)
									  .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
									  .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
									  .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
									  .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
									  .setImage(myImage->GetAPIResource())
									  .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)));
		
		VulkanContext::GetDevice().FlushCommandBuffer(inCommandBuffer);
		VulkanAllocator::DestroyBuffer_TS(stagingBuffer);
		
		myImage->CreateView(vk::ImageViewType::e2D);
#if DEBUG
		VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
															   .setObjectHandle(VulkanContext::GetVulkanHandle(myImage->GetImageView()))
															   .setPObjectName(inImageData.mySourceFile.string().c_str())
															   .setObjectType(vk::ObjectType::eImageView));
#endif
	});

	
}