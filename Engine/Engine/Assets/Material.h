#pragma once
#include "Engine/AssetRegistry/Asset.h"
#include "Engine/Vulkan/VulkanDescriptorSet.h"

class Texture;
class VulkanShader;

class Material : public Asset
{
public:
	Coroutine<void, void, false> Load(const std::filesystem::path inPath) override;

	Coroutine<void, void, false> Load(const std::filesystem::path inPath, const std::filesystem::path inAlbedo, const std::filesystem::path inNormal, const std::filesystem::path inMaterial);
	
	Material() = default;
	
	void Unload() override;
	
	Texture* GetAlbedo() const;
	Texture* GetNormal() const;
	Texture* GetMaterial() const;

private:
	Texture* myAlbedoTexture = nullptr;
	Texture* myNormalTexture = nullptr;
	Texture* myMaterialTexture = nullptr;

	bool myDepthWriteEnabled = true;
};