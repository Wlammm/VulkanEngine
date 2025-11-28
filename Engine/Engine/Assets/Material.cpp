#include "EnginePch.h"
#include "Material.h"
#include "Engine/Engine.h"

#include "Texture.h"
#include "Engine/AssetRegistry/AssetRegistry2.h"

SharedPtr<Texture> Material::GetAlbedo() const
{
	return myAlbedoTexture;
}

SharedPtr<Texture> Material::GetNormal() const
{
	return myNormalTexture;
}

SharedPtr<Texture> Material::GetMaterial() const
{
	return myMaterialTexture;
}

void Material::SetAlbedo(SharedPtr<Texture> inTexture)
{
	myAlbedoTexture = inTexture;
}

void Material::SetAlbedo(const SourcePath& inSourcePath)
{
	myAlbedoTexture = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>(inSourcePath);
}

void Material::SetNormal(SharedPtr<Texture> inTexture)
{
	myNormalTexture = inTexture;
}

void Material::SetNormal(const SourcePath& inSourcePath)
{
	myNormalTexture = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>(inSourcePath);
}

void Material::SetMaterial(SharedPtr<Texture> inTexture)
{
	myMaterialTexture = inTexture;
}

void Material::SetMaterial(const SourcePath& inSourcePath)
{
	myMaterialTexture = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>(inSourcePath);
}

bool Material::GetDepthWriteEnabled() const
{
	return myDepthWriteEnabled;
}
