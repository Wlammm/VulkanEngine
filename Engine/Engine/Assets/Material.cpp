#include "EnginePch.h"
#include "Material.h"
#include "Engine.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include <tracy/Tracy.hpp>

#include "JsonAsset.h"
#include "Texture.h"
#include "Coroutines/Awaitable.h"
#include "World/World.h"

Coroutine<void, void, false> Material::Load(const std::filesystem::path inPath)
{
	JsonAsset* jsonAsset = nullptr;
	co_await Awaitables::WaitForAsset<JsonAsset>(inPath, GetAssetRegistry(), jsonAsset);

	const nlohmann::json& json = jsonAsset->GetJson();

	const std::string albedoPath = json["albedoPath"];
	const std::string normalPath = json["normalPath"];
	const std::string materialPath = json["materialPath"];

	if(!std::filesystem::exists(albedoPath))
	{
		LOG_ERROR("Material requested albedo texture %s but it doesnt exists.", albedoPath.c_str());
		check(false);
	}

	if(!std::filesystem::exists(normalPath))
	{
		LOG_ERROR("Material requested normal texture %s but it doesnt exists.", normalPath.c_str());
		check(false);
	}

	if(!std::filesystem::exists(materialPath))
	{
		LOG_ERROR("Material requested material texture %s but it doesnt exists.", materialPath.c_str());
		check(false);
	}

	co_await Awaitables::WaitForAsset<Texture>(albedoPath, GetAssetRegistry(), myAlbedoTexture);
	co_await Awaitables::WaitForAsset<Texture>(normalPath, GetAssetRegistry(), myNormalTexture);
	co_await Awaitables::WaitForAsset<Texture>(materialPath, GetAssetRegistry(), myMaterialTexture);
	
	co_return;
}

Coroutine<void, void, false> Material::Load(const std::filesystem::path inPath, const std::filesystem::path inAlbedo, const std::filesystem::path inNormal, const std::filesystem::path inMaterial)
{
	if(!std::filesystem::exists(inAlbedo))
	{
		LOG_ERROR("Material requested albedo texture %s but it doesnt exists.", inAlbedo.c_str());
		check(false);
	}

	if(!std::filesystem::exists(inNormal))
	{
		LOG_ERROR("Material requested normal texture %s but it doesnt exists.", inNormal.c_str());
		check(false);
	}

	if(!std::filesystem::exists(inMaterial))
	{
		LOG_ERROR("Material requested material texture %s but it doesnt exists.", inMaterial.c_str());
		check(false);
	}

	co_await Awaitables::WaitForAsset<Texture>(inAlbedo, GetAssetRegistry(), myAlbedoTexture);
	co_await Awaitables::WaitForAsset<Texture>(inNormal, GetAssetRegistry(), myNormalTexture);
	co_await Awaitables::WaitForAsset<Texture>(inMaterial, GetAssetRegistry(), myMaterialTexture);

	co_return;
}

void Material::Unload()
{
	// No need to unload the textures here for now. They're automatically handled by the asset registry.
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