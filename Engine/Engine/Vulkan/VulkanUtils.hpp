#pragma once

#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"

enum class SamplerMode
{
	Clamp,
	Wrap,
	Count
};

class VulkanUtils
{
public:
	static vk::Sampler CreateSampler(SamplerMode inSamplerMode)
	{
		vk::SamplerAddressMode samplerMode;

		if (inSamplerMode == SamplerMode::Clamp)
			samplerMode = vk::SamplerAddressMode::eClampToEdge;

		if (inSamplerMode == SamplerMode::Wrap)
			samplerMode = vk::SamplerAddressMode::eRepeat;

		const auto samplerInfo = vk::SamplerCreateInfo()
			.setMagFilter(vk::Filter::eNearest)
			.setMinFilter(vk::Filter::eNearest)
			.setMipmapMode(vk::SamplerMipmapMode::eNearest)
			.setAddressModeU(samplerMode)
			.setAddressModeV(samplerMode)
			.setAddressModeW(samplerMode)
			.setMipLodBias(0.0f)
			.setAnisotropyEnable(VK_FALSE)
			.setMaxAnisotropy(1)
			.setCompareEnable(VK_FALSE)
			.setCompareOp(vk::CompareOp::eNever)
			.setMinLod(0.0f)
			.setMaxLod(0.0f)
			.setBorderColor(vk::BorderColor::eFloatOpaqueWhite)
			.setUnnormalizedCoordinates(VK_FALSE);

		return VulkanContext::GetDevice()->createSampler(samplerInfo);
	}
};