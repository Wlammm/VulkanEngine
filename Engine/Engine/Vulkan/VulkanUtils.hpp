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
	static void CreateSamplers()
	{
		{
			vk::SamplerAddressMode samplerMode;
			samplerMode = vk::SamplerAddressMode::eClampToEdge;

			const auto samplerInfo = vk::SamplerCreateInfo()
				.setMagFilter(vk::Filter::eNearest)
				.setMinFilter(vk::Filter::eNearest)
				.setMipmapMode(vk::SamplerMipmapMode::eLinear)
				.setAddressModeU(samplerMode)
				.setAddressModeV(samplerMode)
				.setAddressModeW(samplerMode)
				.setMipLodBias(0.0f)
				.setAnisotropyEnable(VK_FALSE)
				.setMaxAnisotropy(1)
				.setCompareEnable(VK_FALSE)
				.setCompareOp(vk::CompareOp::eNever)
				.setMinLod(0.0f)
				.setMaxLod(VK_LOD_CLAMP_NONE)
				.setBorderColor(vk::BorderColor::eFloatOpaqueWhite)
				.setUnnormalizedCoordinates(VK_FALSE);

			mySamplers[SamplerMode::Clamp] = VulkanContext::GetDevice()->createSampler(samplerInfo);
		}

		{
			vk::SamplerAddressMode samplerMode;
			samplerMode = vk::SamplerAddressMode::eRepeat;

			const auto samplerInfo = vk::SamplerCreateInfo()
				.setMagFilter(vk::Filter::eNearest)
				.setMinFilter(vk::Filter::eNearest)
				.setMipmapMode(vk::SamplerMipmapMode::eLinear)
				.setAddressModeU(samplerMode)
				.setAddressModeV(samplerMode)
				.setAddressModeW(samplerMode)
				.setMipLodBias(0.0f)
				.setAnisotropyEnable(VK_FALSE)
				.setMaxAnisotropy(1)
				.setCompareEnable(VK_FALSE)
				.setCompareOp(vk::CompareOp::eNever)
				.setMinLod(0.0f)
				.setMaxLod(VK_LOD_CLAMP_NONE)
				.setBorderColor(vk::BorderColor::eFloatOpaqueWhite)
				.setUnnormalizedCoordinates(VK_FALSE);

			mySamplers[SamplerMode::Wrap] = VulkanContext::GetDevice()->createSampler(samplerInfo);
		}
	}

	static vk::Sampler GetSampler(const SamplerMode inMode)
	{
		return mySamplers[inMode];
	}

	static void DestroySamplers()
	{
		for(auto& [mode, sampler] : mySamplers)
		{
			VulkanContext::GetDevice()->destroySampler(sampler);
			sampler = nullptr;
		}
	}

private:
	static inline std::map<SamplerMode, vk::Sampler> mySamplers{};
};