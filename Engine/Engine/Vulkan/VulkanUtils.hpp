#pragma once

#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"

enum class SamplerMode
{
	LinearClamp,
	LinearWrap,
	PointClamp,
	PointWrap,
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
				.setMagFilter(vk::Filter::eLinear)
				.setMinFilter(vk::Filter::eLinear)
				.setMipmapMode(vk::SamplerMipmapMode::eLinear)
				.setAddressModeU(samplerMode)
				.setAddressModeV(samplerMode)
				.setAddressModeW(samplerMode)
				.setMipLodBias(0.0f)
				.setAnisotropyEnable(VK_TRUE)
				.setMaxAnisotropy(16)
				.setCompareEnable(VK_FALSE)
				.setCompareOp(vk::CompareOp::eNever)
				.setMinLod(0.0f)
				.setMaxLod(VK_LOD_CLAMP_NONE)
				.setBorderColor(vk::BorderColor::eFloatOpaqueWhite)
				.setUnnormalizedCoordinates(VK_FALSE);

			mySamplers[SamplerMode::LinearClamp] = VulkanContext::GetDevice()->createSampler(samplerInfo);
		}
		
		{
			vk::SamplerAddressMode samplerMode;
			samplerMode = vk::SamplerAddressMode::eClampToEdge;

			const auto samplerInfo = vk::SamplerCreateInfo()
				.setMagFilter(vk::Filter::eNearest)
				.setMinFilter(vk::Filter::eNearest)
				.setMipmapMode(vk::SamplerMipmapMode::eNearest)
				.setAddressModeU(samplerMode)
				.setAddressModeV(samplerMode)
				.setAddressModeW(samplerMode)
				.setMipLodBias(0.0f)
				.setAnisotropyEnable(VK_TRUE)
				.setMaxAnisotropy(16)
				.setCompareEnable(VK_FALSE)
				.setCompareOp(vk::CompareOp::eNever)
				.setMinLod(0.0f)
				.setMaxLod(VK_LOD_CLAMP_NONE)
				.setBorderColor(vk::BorderColor::eFloatOpaqueWhite)
				.setUnnormalizedCoordinates(VK_FALSE);

			mySamplers[SamplerMode::PointClamp] = VulkanContext::GetDevice()->createSampler(samplerInfo);
		}

		{
			vk::SamplerAddressMode samplerMode;
			samplerMode = vk::SamplerAddressMode::eRepeat;

			const auto samplerInfo = vk::SamplerCreateInfo()
				.setMagFilter(vk::Filter::eLinear)
				.setMinFilter(vk::Filter::eLinear)
				.setMipmapMode(vk::SamplerMipmapMode::eLinear)
				.setAddressModeU(samplerMode)
				.setAddressModeV(samplerMode)
				.setAddressModeW(samplerMode)
				.setMipLodBias(0.0f)
				.setAnisotropyEnable(VK_TRUE)
				.setMaxAnisotropy(16)
				.setCompareEnable(VK_FALSE)
				.setCompareOp(vk::CompareOp::eNever)
				.setMinLod(0.0f)
				.setMaxLod(VK_LOD_CLAMP_NONE)
				.setBorderColor(vk::BorderColor::eFloatOpaqueWhite)
				.setUnnormalizedCoordinates(VK_FALSE);

			mySamplers[SamplerMode::LinearWrap] = VulkanContext::GetDevice()->createSampler(samplerInfo);
		}
		
		{
			vk::SamplerAddressMode samplerMode;
			samplerMode = vk::SamplerAddressMode::eRepeat;

			const auto samplerInfo = vk::SamplerCreateInfo()
				.setMagFilter(vk::Filter::eNearest)
				.setMinFilter(vk::Filter::eNearest)
				.setMipmapMode(vk::SamplerMipmapMode::eNearest)
				.setAddressModeU(samplerMode)
				.setAddressModeV(samplerMode)
				.setAddressModeW(samplerMode)
				.setMipLodBias(0.0f)
				.setAnisotropyEnable(VK_TRUE)
				.setMaxAnisotropy(16)
				.setCompareEnable(VK_FALSE)
				.setCompareOp(vk::CompareOp::eNever)
				.setMinLod(0.0f)
				.setMaxLod(VK_LOD_CLAMP_NONE)
				.setBorderColor(vk::BorderColor::eFloatOpaqueWhite)
				.setUnnormalizedCoordinates(VK_FALSE);

			mySamplers[SamplerMode::PointWrap] = VulkanContext::GetDevice()->createSampler(samplerInfo);
		}

		RegisterNamedSamplers();
	}

	static vk::Sampler GetSampler(const SamplerMode inMode)
	{
		return mySamplers[inMode];
	}

	static vk::Sampler TryGetSampler(const std::string& inName)
	{
		auto it = myNamedSamplers.find(inName);
		return it != myNamedSamplers.end() ? it->second : vk::Sampler{};
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
	static void RegisterNamedSamplers()
	{
		myNamedSamplers["linearWrapSampler"]  = mySamplers[SamplerMode::LinearWrap];
		myNamedSamplers["linearClampSampler"] = mySamplers[SamplerMode::LinearClamp];
		myNamedSamplers["pointWrapSampler"]   = mySamplers[SamplerMode::PointWrap];
		myNamedSamplers["pointClampSampler"]  = mySamplers[SamplerMode::PointClamp];
	}

	static inline std::map<SamplerMode, vk::Sampler> mySamplers{};
	static inline std::unordered_map<std::string, vk::Sampler> myNamedSamplers{};
};