#pragma once
#include "VulkanUtils.hpp"

class VulkanImage
{
public:
	~VulkanImage();

	operator vk::Image();
	vk::Image operator->();

	vk::ImageView GetImageView() const;
	vk::Format GetFormat() const;
	vk::Sampler GetSampler() const;

	void CreateView(vk::ImageViewType inViewType);
	void CreateView(vk::ImageViewType inViewType, vk::ImageSubresourceRange inRange);
	void CreateSampler(SamplerMode inSamplerMode);

private:
	void* Map();
	void Unmap();

private:
	friend class VulkanAllocator;
	VmaAllocation myAllocation;

	vk::Image myImage;
	vk::ImageView myView;
	vk::Format myFormat;
	vk::Sampler mySampler;

#ifdef DEBUG
	std::string myName = "";
#endif
};