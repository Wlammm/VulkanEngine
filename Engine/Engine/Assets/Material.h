#pragma once
#include "Vulkan/VulkanDescriptorSet.h"

class Texture;
class VulkanShader;

class Material
{
public:
	Material();
	Material(const std::filesystem::path& inAlbedo, const std::filesystem::path& inNormal, const std::filesystem::path& inMaterial);
	~Material();

	Texture* GetAlbedo() const;
	Texture* GetNormal() const;
	Texture* GetMaterial() const;

	bool IsValid() const { return myIsValid;}
	
private:
	bool myIsValid = false;
	Texture* myAlbedoTexture;
	Texture* myNormalTexture;
	Texture* myMaterialTexture;
};