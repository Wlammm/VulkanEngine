#include "EnginePch.h"
#include "Material.h"
#include "Engine.h"
#include "Engine/Assets/AssetRegistry.h"
#include <tracy/Tracy.hpp>

#include "Vulkan/VulkanUtils.hpp"

Material::Material()
{
	ZoneScoped;
	myAlbedo = Engine::GetAssetRegistry().GetImage("Assets/Leaves.tga");
	myNormal = Engine::GetAssetRegistry().GetImage("Assets/Leaves.tga");
	myMaterial = Engine::GetAssetRegistry().GetImage("Assets/Leaves.tga");

	BuildDescriptorSet();
}

Material::Material(const std::filesystem::path& inAlbedo, const std::filesystem::path& inNormal, const std::filesystem::path& inMaterial)
{
	ZoneScoped;
	myAlbedoPath = inAlbedo;
	myAlbedo = Engine::GetAssetRegistry().GetImage(inAlbedo);

	myNormalPath = inNormal;
	myNormal = Engine::GetAssetRegistry().GetImage(inNormal);

	myMaterialPath = inMaterial;
	myMaterial = Engine::GetAssetRegistry().GetImage(inMaterial);

	BuildDescriptorSet();
}

Material::~Material()
{
}

vk::DescriptorSet Material::GetDescriptorSet()
{
	return myDescriptorSet.GetSet();
}

TextureHandle Material::GetAlbedo() const
{
	return myAlbedoHandle;
}

TextureHandle Material::GetNormal() const
{
	return myNormalHandle;
}

TextureHandle Material::GetMaterial() const
{
	return myMaterialHandle;
}

void Material::BuildDescriptorSet()
{
	ZoneScoped;
	TextureSystem& textureSystem = Engine::GetEngineSystem<TextureSystem>();

	myAlbedoHandle = textureSystem.AddTexture(myAlbedo, VulkanUtils::GetSampler(SamplerMode::Wrap));
	myNormalHandle = textureSystem.AddTexture(myNormal, VulkanUtils::GetSampler(SamplerMode::Wrap));
	myMaterialHandle = textureSystem.AddTexture(myMaterial, VulkanUtils::GetSampler(SamplerMode::Wrap));
	
	myDescriptorSet.BindImage(myAlbedo, VulkanUtils::GetSampler(SamplerMode::Wrap), 0, vk::ShaderStageFlagBits::eFragment);
	myDescriptorSet.BindImage(myNormal, VulkanUtils::GetSampler(SamplerMode::Wrap), 1, vk::ShaderStageFlagBits::eFragment);
	myDescriptorSet.BindImage(myMaterial, VulkanUtils::GetSampler(SamplerMode::Wrap), 2, vk::ShaderStageFlagBits::eFragment);
	myDescriptorSet.Build();
}
