#pragma once
#include "VulkanUtils.hpp"

class VulkanImage
{
public:
	static vk::ImageCreateInfo DepthCreateInfo(const glm::vec2& inResolution, vk::SampleCountFlagBits inSampleCount = vk::SampleCountFlagBits::e1)
	{
		const vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
			.setImageType(vk::ImageType::e2D)
			.setFormat(vk::Format::eD32Sfloat)
			.setExtent({ static_cast<uint>(inResolution.x), static_cast<uint>(inResolution.y), 1 })
			.setMipLevels(1)
			.setArrayLayers(1)
			.setSamples(inSampleCount)
			.setTiling(vk::ImageTiling::eOptimal)
#if EDITOR
			// We need to add transfer source when in editor to be able to readback depth for editor stuff.
			// TODO: Only do this for the resolve depth. Not required for all depth buffers
			.setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eTransferSrc)
#else
			.setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment)
#endif
			.setSharingMode(vk::SharingMode::eExclusive)
			.setInitialLayout(vk::ImageLayout::eUndefined);

		return createInfo;
	}

	static vk::ImageCreateInfo ShadowMapCreateInfo(const glm::vec2& inResolution)
	{
		const vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
			.setImageType(vk::ImageType::e2D)
			.setFormat(vk::Format::eD32Sfloat)
			.setExtent({ static_cast<uint>(inResolution.x), static_cast<uint>(inResolution.y), 1 })
			.setMipLevels(1)
			.setArrayLayers(1)
			.setSamples(vk::SampleCountFlagBits::e1)
			.setTiling(vk::ImageTiling::eOptimal)
			.setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eSampled)
			.setSharingMode(vk::SharingMode::eExclusive)
			.setInitialLayout(vk::ImageLayout::eUndefined);

		return createInfo;
	}

	static VulkanImage* CreateFromSwapchainImage(vk::Image inImage, vk::Format inFormat);
	static void DestroySwapchainImage(VulkanImage* inImage);
	
public:
	vk::Image GetAPIResource() const;
	vk::ImageView GetImageView() const;
	vk::Format GetFormat() const;

	bool HasImageView() const;
	void CreateView(vk::ImageViewType inViewType);
	void CreateView(vk::ImageViewType inViewType, vk::ImageSubresourceRange inRange);

	void CreateDepthView();

	const glm::vec2& GetSize() const;
	uint GetNumMipLevels() const;
	vk::SampleCountFlagBits GetNumMSAASamples() const;
	
	vk::ImageSubresourceRange GetSubresourceRange() const;
	
private:
	// Only create & destroy via allocator.
	VulkanImage() = default;
	~VulkanImage();
	VulkanImage(const VulkanImage& inOther) = delete;

	void* Map();
	void Unmap();

private:
	friend class VulkanAllocator;
	VmaAllocation myAllocation;

	vk::Image myImage;
	vk::ImageView myView;
	vk::Format myFormat;
	glm::vec2 mySize;
	
	vk::ImageSubresourceRange mySubresourceRange;
	
    uint myNumMipLevels = 1;
	vk::SampleCountFlagBits myNumMSAASamples = vk::SampleCountFlagBits::e1;

#ifdef DEBUG
	std::string myName = "";
#endif
};