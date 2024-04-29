#pragma once
#include "AssetObserver.h"
#include "Rendering/TextureSystem.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanDescriptorSet.h"

class VulkanShader;

class Material
{
public:
	static vk::DescriptorSetLayout GetMaterialDescriptorLayout()
	{
		static vk::DescriptorSetLayout layout;

		DO_ONCE(
			List<vk::DescriptorSetLayoutBinding> layoutBindings;

			// Albedo binding.
			layoutBindings.Emplace()
				.setBinding(0)
				.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
				.setDescriptorCount(1)
				.setStageFlags(vk::ShaderStageFlagBits::eFragment);
			// Normal binding.
			layoutBindings.Emplace()
				.setBinding(1)
				.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
				.setDescriptorCount(1)
				.setStageFlags(vk::ShaderStageFlagBits::eFragment);
			// Material binding.
			layoutBindings.Emplace()
				.setBinding(2)
				.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
				.setDescriptorCount(1)
				.setStageFlags(vk::ShaderStageFlagBits::eFragment);

			layout = VulkanContext::GetDevice()->createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo().setBindings(layoutBindings));
		);
		return layout;
	}

public:
	Material();
	Material(const std::filesystem::path& inAlbedo, const std::filesystem::path& inNormal, const std::filesystem::path& inMaterial);
	~Material();

	vk::DescriptorSet GetDescriptorSet();

	TextureHandle GetAlbedo() const;
	TextureHandle GetNormal() const;
	TextureHandle GetMaterial() const;

private:
	void BuildDescriptorSet();

private:
	class VulkanImage* myAlbedo = nullptr;
	class VulkanImage* myNormal = nullptr;
	class VulkanImage* myMaterial = nullptr;

	std::filesystem::path myAlbedoPath;
	std::filesystem::path myNormalPath;
	std::filesystem::path myMaterialPath;

	TextureHandle myAlbedoHandle;
	TextureHandle myNormalHandle;
	TextureHandle myMaterialHandle;

	VulkanDescriptorSet myDescriptorSet = VulkanDescriptorSet{ Material::GetMaterialDescriptorLayout() };
};