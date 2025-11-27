#include "EnginePch.h"
#include "Material.h"
#include "Engine/Engine.h"

#include "JsonAsset.h"
#include "Texture.h"

void Material::PostPropertiesSerialized()
{
	Asset2::PostPropertiesSerialized();
	
	if (!myAlbedoPath.empty())
		myAlbedoTexture = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>(myAlbedoPath);
	
	if (!myNormalPath.empty())
		myNormalTexture = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>(myNormalPath);
	
	if (!myMaterialPath.empty())
		myMaterialTexture = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>(myMaterialPath);
}

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
	myAlbedoPath = inTexture->GetSourcePath();
}

void Material::SetAlbedo(const SourcePath& inSourcePath)
{
	myAlbedoPath = inSourcePath;
	myAlbedoTexture = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>(myAlbedoPath);
}

void Material::SetNormal(SharedPtr<Texture> inTexture)
{
	myNormalTexture = inTexture;
	myNormalPath = inTexture->GetSourcePath();
}

void Material::SetNormal(const SourcePath& inSourcePath)
{
	myNormalPath = inSourcePath;
	myNormalTexture = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>(inSourcePath);
}

void Material::SetMaterial(SharedPtr<Texture> inTexture)
{
	myMaterialTexture = inTexture;
	myMaterialPath = inTexture->GetSourcePath();
}

void Material::SetMaterial(const SourcePath& inSourcePath)
{
	myMaterialPath = inSourcePath;
	myMaterialTexture = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>(inSourcePath);
}

bool Material::GetDepthWriteEnabled() const
{
	return myDepthWriteEnabled;
}
