#pragma once

class VulkanRenderImages
{
public:
	VulkanRenderImages(const vk::ImageCreateInfo& inCreateInfo);

	vk::ImageView GetView(const uint inIndex);
	vk::Format GetFormat() const;

private:
	List<class VulkanImage*> myImages{};
};