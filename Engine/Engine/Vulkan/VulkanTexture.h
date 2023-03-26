#pragma once

#include "VulkanUTils.hpp"

class VulkanTexture
{
public:
	VulkanTexture(const std::filesystem::path& inPath, SamplerMode inSamplerMode);
	~VulkanTexture();

	vk::Sampler GetSampler() const;
	vk::ImageView GetImageView() const;

private:
	void CreateImage(const std::filesystem::path& inPath);
	void CreateSampler(SamplerMode inSamplerMode);

private:
	class VulkanImage* myImage = nullptr;

	vk::Sampler mySampler;
};