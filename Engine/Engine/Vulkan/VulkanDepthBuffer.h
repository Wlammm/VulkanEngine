#pragma once

class VulkanDepthBuffer
{
public:
	VulkanDepthBuffer(const Vec2ui& inResolution);
	~VulkanDepthBuffer();

	vk::ImageView GetImageView() const;
	vk::Format GetFormat() const;

private:
	class VulkanImage* myImage = nullptr;
};