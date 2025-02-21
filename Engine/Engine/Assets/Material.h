#pragma once
#include "AssetRegistry/Asset.h"
#include "Vulkan/VulkanDescriptorSet.h"

class Texture;
class VulkanShader;

class Material : public Asset
{
public:
	Coroutine<void, void, false> Load(const std::filesystem::path inPath) override;

	Material() = default;
	Material(World* inWorld, const std::filesystem::path& inAlbedo, const std::filesystem::path& inNormal, const std::filesystem::path& inMaterial);
	
	void Unload() override;
	
	Texture* GetAlbedo() const;
	Texture* GetNormal() const;
	Texture* GetMaterial() const;
	
private:
	Texture* myAlbedoTexture = nullptr;
	Texture* myNormalTexture = nullptr;
	Texture* myMaterialTexture = nullptr;
};