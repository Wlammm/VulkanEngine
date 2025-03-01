#include "EnginePch.h"
#include "TextureCube.h"

#include <stb/stb_image.h>

#include "Rendering/RenderSystem.h"
#include "Rendering/TextureSystem.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanImage.h"
#include "Vulkan/Staging/StagingBuffer.h"
#include "Vulkan/Staging/StagingSystem.h"

// TODO: Do we need full 32 bit resolution here? 
const vk::Format IMAGE_FORMAT = vk::Format::eR32G32B32A32Sfloat;

Coroutine<void, void, false> TextureCube::Load(const std::filesystem::path inPath)
{
    int width, height, numComponents;
    float* data = stbi_loadf(inPath.string().c_str(), &width, &height, &numComponents, 0);

    vk::BufferCreateInfo stagingCreateInfo = vk::BufferCreateInfo()
            .setSize(sizeof(float) * width * height * 4)
            .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
            .setSharingMode(vk::SharingMode::eExclusive);
    VulkanBuffer* stagingBuffer = VulkanAllocator::AllocateBuffer_TS("Texture staging buffer", stagingCreateInfo, VMA_MEMORY_USAGE_AUTO, true);

    vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
        .setImageType(vk::ImageType::e2D)
        .setFormat(IMAGE_FORMAT)
        .setExtent({ static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 })
        .setMipLevels(1)
        .setArrayLayers(1)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setTiling(vk::ImageTiling::eOptimal)
        .setUsage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eSampled)
        .setSharingMode(vk::SharingMode::eExclusive)
        .setInitialLayout(vk::ImageLayout::eUndefined);

    std::string imageName = "TextureCube - " + inPath.string();
    myImage = VulkanAllocator::AllocateImage_TS(imageName, createInfo, VMA_MEMORY_USAGE_AUTO);

#if DEBUG
    VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
                                                           .setObjectHandle(VulkanContext::GetVulkanHandle(myImage->GetAPIResource()))
                                                           .setPObjectName(inPath.string().c_str())
                                                           .setObjectType(vk::ObjectType::eImage));
#endif

    myImage->CreateView(vk::ImageViewType::e2D);

    vk::CommandBuffer commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();

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
	
    commandBuffer.copyBufferToImage(stagingBuffer->GetAPIResource(), myImage->GetAPIResource(), vk::ImageLayout::eTransferDstOptimal, { bufferCopyRegion });
    RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer);
	VulkanAllocator::DestroyBuffer_TS(stagingBuffer);

    TextureSystem::RegisterTextureCube_TS(this);
    
    co_return;
}

void TextureCube::Unload()
{
    VulkanAllocator::DestroyImage_TS(myImage);
    myImage = nullptr;
}

vk::ImageView TextureCube::GetImageView() const
{
    return myImage->GetImageView();
}

uint TextureCube::GetBindlessIndex() const
{
    return myBindlessIndex;
}
