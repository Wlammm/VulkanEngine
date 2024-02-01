#pragma once
#include "VulkanUtils.hpp"

class VulkanImage
{
public:
	static vk::ImageCreateInfo DepthCreateInfo(const Vec2ui& inResolution)
	{
		const vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
			.setImageType(vk::ImageType::e2D)
			.setFormat(vk::Format::eD32Sfloat)
			.setExtent({ inResolution.x, inResolution.y, 1 })
			.setMipLevels(1)
			.setArrayLayers(1)
			.setSamples(vk::SampleCountFlagBits::e1)
			.setTiling(vk::ImageTiling::eOptimal)
			.setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment)
			.setSharingMode(vk::SharingMode::eExclusive)
			.setInitialLayout(vk::ImageLayout::eUndefined);

		return createInfo;
	}

	static vk::ImageCreateInfo ShadowMapCreateInfo(const Vec2ui& inResolution)
	{
		const vk::ImageCreateInfo createInfo = vk::ImageCreateInfo()
			.setImageType(vk::ImageType::e2D)
			.setFormat(vk::Format::eD32Sfloat)
			.setExtent({ inResolution.x, inResolution.y, 1 })
			.setMipLevels(1)
			.setArrayLayers(1)
			.setSamples(vk::SampleCountFlagBits::e1)
			.setTiling(vk::ImageTiling::eOptimal)
			.setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eSampled)
			.setSharingMode(vk::SharingMode::eExclusive)
			.setInitialLayout(vk::ImageLayout::eUndefined);

		return createInfo;
	}

public:
	vk::Image GetAPIResource() const;
	vk::ImageView GetImageView() const;
	vk::Format GetFormat() const;

	void CreateView(vk::ImageViewType inViewType);
	void CreateView(vk::ImageViewType inViewType, vk::ImageSubresourceRange inRange);

	void CreateDepthView();

	const Vec2ui& GetSize() const;

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
	Vec2ui mySize;

#ifdef DEBUG
	std::string myName = "";
#endif
};