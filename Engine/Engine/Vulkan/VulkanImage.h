#pragma once

class VulkanImage
{
public:
	~VulkanImage();

	void CreateView(vk::ImageViewType inViewType, vk::ImageSubresourceRange inSubResourceRange);

	operator vk::Image();

	vk::Image operator->();

	vk::ImageView GetImageView() const;

	vk::Format GetFormat();

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