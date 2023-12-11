#include "EnginePch.h"
#include "Material.h"
#include "Engine.h"
#include "ECS/Systems/RenderSystem.h"
#include "Engine/Assets/AssetRegistry.h"

#include "Vulkan/VulkanPipeline.h"
#include "Vulkan/VulkanShader.h"
#include "Vulkan/VulkanImage.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanAllocator.h"

#include <tracy/Tracy.hpp>

Material::Material()
{
	ZoneScoped;
	myAlbedo = VulkanImage::LoadFromFile("Assets/Leaves.tga");
	myAlbedo->CreateSampler(SamplerMode::Wrap);
	
	myNormal = VulkanImage::LoadFromFile("Assets/Leaves.tga");
	myNormal->CreateSampler(SamplerMode::Wrap);

	myMaterial = VulkanImage::LoadFromFile("Assets/Leaves.tga");
	myMaterial->CreateSampler(SamplerMode::Wrap);

	BuildDescriptorSet();
}

Material::Material(const std::filesystem::path& inAlbedo, const std::filesystem::path& inNormal, const std::filesystem::path& inMaterial)
{
	ZoneScoped;
	myAlbedoPath = inAlbedo;
	myAlbedo = Engine::GetAssetRegistry().GetImage(inAlbedo);
	myAlbedo->CreateSampler(SamplerMode::Wrap);

	myNormalPath = inNormal;
	myNormal = Engine::GetAssetRegistry().GetImage(inNormal);
	myNormal->CreateSampler(SamplerMode::Wrap);

	myMaterialPath = inMaterial;
	myMaterial = Engine::GetAssetRegistry().GetImage(inMaterial);
	myMaterial->CreateSampler(SamplerMode::Wrap);

	BuildDescriptorSet();
}

Material::~Material()
{
}

vk::DescriptorSet Material::GetDescriptorSet()
{
	return myDescriptorSet.GetSet();
}

void Material::BuildDescriptorSet()
{
	ZoneScoped;
	myDescriptorSet.BindImage(myAlbedo, 0, vk::ShaderStageFlagBits::eFragment);
	myDescriptorSet.BindImage(myNormal, 1, vk::ShaderStageFlagBits::eFragment);
	myDescriptorSet.BindImage(myMaterial, 2, vk::ShaderStageFlagBits::eFragment);
	myDescriptorSet.Build();
}