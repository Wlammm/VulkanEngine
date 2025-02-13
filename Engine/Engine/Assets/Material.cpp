#include "EnginePch.h"
#include "Material.h"
#include "Engine.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include <tracy/Tracy.hpp>

#include "Texture.h"

Material::Material()
{
	ZoneScoped;
	
	//Engine::GetAssetRegistry().GetAssetAsync<Texture>("Assets/Leaves.tga", [&](Texture* inTexture)
	//{
	//	myAlbedoTexture = inTexture;
	//});
//
	//Engine::GetAssetRegistry().GetAssetAsync<Texture>("Assets/Leaves.tga", [&](Texture* inTexture)
	//{
	//	myNormalTexture = inTexture;
	//});
//
	//Engine::GetAssetRegistry().GetAssetAsync<Texture>("Assets/Leaves.tga", [&](Texture* inTexture)
	//{
	//	myMaterialTexture = inTexture;
	//});
}

Material::Material(const std::filesystem::path& inAlbedo, const std::filesystem::path& inNormal, const std::filesystem::path& inMaterial)
{
	ZoneScoped;
	myIsValid = true;
/*
	Engine::GetAssetRegistry().GetAssetAsync<Texture>(inAlbedo, [&](Texture* inTexture)
	{
		myAlbedoTexture = inTexture;
	});

	Engine::GetAssetRegistry().GetAssetAsync<Texture>(inNormal, [&](Texture* inTexture)
	{
		myNormalTexture = inTexture;
	});

	Engine::GetAssetRegistry().GetAssetAsync<Texture>(inMaterial, [&](Texture* inTexture)
	{
		myMaterialTexture = inTexture;
	});*/

	// TODO: Need to fix material system and asset system to not have race conditions.
	myAlbedoTexture = Engine::GetAssetRegistry().GetAssetSynchronous<Texture>(inAlbedo);
	myNormalTexture = Engine::GetAssetRegistry().GetAssetSynchronous<Texture>(inNormal);
	myMaterialTexture = Engine::GetAssetRegistry().GetAssetSynchronous<Texture>(inMaterial);
}

Material::~Material()
{
}

Texture* Material::GetAlbedo() const
{
	return myAlbedoTexture;
}

Texture* Material::GetNormal() const
{
	return myNormalTexture;
}

Texture* Material::GetMaterial() const
{
	return myMaterialTexture;
}