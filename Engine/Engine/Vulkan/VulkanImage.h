#pragma once
#include "VulkanUtils.hpp"

class VulkanImage
{
public:
	vk::Image GetAPIResource() const;
	vk::ImageView GetImageView() const;
	vk::Format GetFormat() const;

	void CreateView(vk::ImageViewType inViewType);
	void CreateView(vk::ImageViewType inViewType, vk::ImageSubresourceRange inRange);

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

#ifdef DEBUG
	std::string myName = "";
#endif
};