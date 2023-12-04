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

Material::Material()
{
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
	myAlbedoPath = inAlbedo;
	myAlbedo = VulkanImage::LoadFromFile(inAlbedo);
	myAlbedo->CreateSampler(SamplerMode::Wrap);

	myNormalPath = inNormal;
	myNormal = VulkanImage::LoadFromFile(inNormal);
	myNormal->CreateSampler(SamplerMode::Wrap);

	myMaterialPath = inMaterial;
	myMaterial = VulkanImage::LoadFromFile(inMaterial);
	myMaterial->CreateSampler(SamplerMode::Wrap);

	BuildDescriptorSet();
}

Material::~Material()
{
	VulkanContext::GetAllocator().DestroyImage(myAlbedo);
	VulkanContext::GetAllocator().DestroyImage(myNormal);
	VulkanContext::GetAllocator().DestroyImage(myMaterial);
}

vk::DescriptorSet Material::GetDescriptorSet()
{
	return myDescriptorSet.GetSet();
}

void Material::BuildDescriptorSet()
{
	myDescriptorSet.BindImage(myAlbedo, 0, vk::ShaderStageFlagBits::eFragment);
	myDescriptorSet.BindImage(myNormal, 1, vk::ShaderStageFlagBits::eFragment);
	myDescriptorSet.BindImage(myMaterial, 2, vk::ShaderStageFlagBits::eFragment);
	myDescriptorSet.Build();
}