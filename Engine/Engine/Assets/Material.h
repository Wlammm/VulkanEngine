#pragma once
#include "AssetObserver.h"

class VulkanPipeline;
class VulkanShader;

class Material : public AssetObserver
{
public:
	static vk::DescriptorSetLayout GetMaterialDescriptorLayout()
	{
		static vk::DescriptorSetLayout layout;

		DO_ONCE(
			List<vk::DescriptorSetLayoutBinding> layoutBindings;

			// Albedo binding.
			layoutBindings.Emplace()
				.setBinding(1)
				.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
				.setDescriptorCount(1)
				.setStageFlags(vk::ShaderStageFlagBits::eFragment);
			// Normal binding.
			layoutBindings.Emplace()
				.setBinding(2)
				.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
				.setDescriptorCount(1)
				.setStageFlags(vk::ShaderStageFlagBits::eFragment);
			// Material binding.
			layoutBindings.Emplace()
				.setBinding(3)
				.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
				.setDescriptorCount(1)
				.setStageFlags(vk::ShaderStageFlagBits::eFragment);

			layout = VulkanContext::GetDevice()->createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo().setBindings(layoutBindings));
		);
		return layout;
	}

public:
	Material();
	~Material();

	void Create();
	void Destroy();

	void Bind(vk::CommandBuffer inCommandBuffer) const;

private:
	VulkanPipeline* myPipeline;
	VulkanShader* myVertexShader;
	VulkanShader* myPixelShader;

	class VulkanTexture* myTexture = nullptr;


	// Inherited via AssetObserver
	virtual void OnAssetUpdated() override;

	/*
	* VertexShader
	* FragmentShader
	* Albedo
	* */
};