#include "EnginePch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Engine/Rendering/RenderSystem.h"
#include "Engine/Rendering/TextureSystem.h"
#include "Engine/Serialization/BinarySerializer.h"
#include "Engine/Utils/ThreadUtils.hpp"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanCommandBuffer.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanImage.h"
#include "Engine/Vulkan/VulkanPhysicalDevice.h"

const vk::Format IMAGE_FORMAT = vk::Format::eR8G8B8A8Unorm;


Texture::~Texture()
{
    if (!IsValid())
        return;
    
    TextureSystem::RemoveTexture_TS(this);
    VulkanAllocator::DestroyImage_TS(myImage);
    myImage = nullptr;
}

void Texture::LoadPropertiesFromSource()
{
    ZoneScoped;

    Asset2::LoadPropertiesFromSource();

    stbi_uc* pixels = stbi_load(GetSourcePath().string().c_str(), &myImageData.myWidth, &myImageData.myHeight, &myImageData.myChannels,
                                STBI_rgb_alpha);
    check(pixels);
    check(myImageData.myWidth != 0 && myImageData.myHeight != 0);

    myImageData.myNumMipLevels = static_cast<uint>(std::floor(std::log2(std::max(myImageData.myHeight, myImageData.myWidth))))
        + 1;

    myImageData.myPixelData.Resize(myImageData.myWidth * myImageData.myHeight * 4);
    memcpy(myImageData.myPixelData.data(), pixels, myImageData.myPixelData.size());
    stbi_image_free(pixels);
}

void Texture::PostPropertiesSerialized()
{
    Asset2::PostPropertiesSerialized();
    
     ZoneScoped;
    vk::BufferCreateInfo stagingCreateInfo = vk::BufferCreateInfo()
                                             .setSize(sizeof(size_t) * myImageData.myWidth * myImageData.myHeight)
                                             .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
                                             .setSharingMode(vk::SharingMode::eExclusive);
    VulkanBuffer* stagingBuffer = VulkanAllocator::AllocateBuffer_TS("Texture staging buffer", stagingCreateInfo,
                                                                     VMA_MEMORY_USAGE_AUTO, true);

    void* ptr = stagingBuffer->GetPtr();
    //check(myImageData.myChannels == 4); // Make sure channels is 4. We dont support anything else atm.
    memcpy(ptr, myImageData.myPixelData.data(), myImageData.myWidth * myImageData.myHeight * 4);

    vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
                                     .setImageType(vk::ImageType::e2D)
                                     .setFormat(IMAGE_FORMAT)
                                     .setExtent({
                                         static_cast<uint32_t>(myImageData.myWidth),
                                         static_cast<uint32_t>(myImageData.myHeight), 1
                                     })
                                     .setMipLevels(myImageData.myNumMipLevels)
                                     .setArrayLayers(1)
                                     .setSamples(vk::SampleCountFlagBits::e1)
                                     .setTiling(vk::ImageTiling::eOptimal)
                                     .setUsage(
                                         vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc |
                                         vk::ImageUsageFlagBits::eSampled)
                                     .setSharingMode(vk::SharingMode::eExclusive)
                                     .setInitialLayout(vk::ImageLayout::eUndefined);

    std::string imageName = "VulkanImage - " + GetSourcePath().string();
    myImage = VulkanAllocator::AllocateImage_TS(imageName, createInfo, VMA_MEMORY_USAGE_AUTO);

#if DEBUG
    VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
                                                           .setObjectHandle(
                                                               VulkanContext::GetVulkanHandle(
                                                                   myImage->GetAPIResource()))
                                                           .setPObjectName(GetSourcePath().string().c_str())
                                                           .setObjectType(vk::ObjectType::eImage));
#endif
    myImage->CreateView(vk::ImageViewType::e2D);

    VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();

    vk::ImageSubresourceRange subresourceRange = vk::ImageSubresourceRange()
                                                 .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                                 .setBaseMipLevel(0)
                                                 .setLevelCount(myImageData.myNumMipLevels)
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
    commandBuffer->GetAPIResource().pipelineBarrier(vk::PipelineStageFlagBits::eHost, vk::PipelineStageFlagBits::eTransfer,
                                  vk::DependencyFlagBits(), {}, {}, {imageMemoryBarrier});

    vk::BufferImageCopy bufferCopyRegion{};
    bufferCopyRegion.imageSubresource
                    .setAspectMask(vk::ImageAspectFlagBits::eColor)
                    .setMipLevel(0)
                    .setBaseArrayLayer(0)
                    .setLayerCount(1);
    bufferCopyRegion.imageExtent
                    .setWidth(myImageData.myWidth)
                    .setHeight(myImageData.myHeight)
                    .setDepth(1);
    bufferCopyRegion.setBufferOffset(0);

    commandBuffer->GetAPIResource().copyBufferToImage(stagingBuffer->GetAPIResource(), myImage->GetAPIResource(),
                                    vk::ImageLayout::eTransferDstOptimal, {bufferCopyRegion});

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


    GenerateMipLevels(commandBuffer->GetAPIResource());
    RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer);
    VulkanAllocator::DestroyBuffer_TS(stagingBuffer);
#if DEBUG
    VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
                                                           .setObjectHandle(
                                                               VulkanContext::GetVulkanHandle(myImage->GetImageView()))
                                                           .setPObjectName(GetSourcePath().string().c_str())
                                                           .setObjectType(vk::ObjectType::eImageView));
#endif
    TextureSystem::RegisterTexture_TS(this);
    
    // Reset image data here to save memory as it's not needed anymore.
    myImageData = {};
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

    for (uint i = 1; i < myImage->GetNumMipLevels(); ++i)
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
        blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};
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