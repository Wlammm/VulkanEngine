#include "EnginePch.h"
#include "AssetRegistry.h"
#include "Model.h"
#include "Vulkan/VulkanShader.h"
#include "Material.h"

AssetRegistry::AssetRegistry()
{
}

AssetRegistry::~AssetRegistry()
{
	for(auto& [path, model] : myModels)
	{
		del(model);
	}
	myModels.clear();

	for (auto& [path, material] : myMaterials)
	{
		del(material);
	}
	myMaterials.clear();

	for (auto& [path, shader] : myShaders)
	{
		del(shader);
	}
	myShaders.clear();
}

void AssetRegistry::Init()
{
	myMaterials["default"] = new Material();
}

Model* AssetRegistry::GetModel(const std::filesystem::path& inPath, const Model::CreateInfo& inCreateInfo)
{
	if (!std::filesystem::exists(inPath))
	{
		LOG_WARNING("AssetRegistry::GetModel: Tried to load model with a filepath that doesnt exists.");
		return nullptr;
	}

	if(myModels.find(inPath) != myModels.end())
		return myModels[inPath];

	myModels[inPath] = new Model(inPath, inCreateInfo);
	return myModels[inPath];
}

VulkanShader* AssetRegistry::GetShader(const std::filesystem::path& inPath)
{
	if (myShaders.find(inPath) != myShaders.end())
		return myShaders[inPath];

	myShaders[inPath] = new VulkanShader(inPath);
	return myShaders[inPath];
}

Material* AssetRegistry::GetMaterial(const std::filesystem::path& inPath)
{
	check(false);
	return nullptr;
}

Material* AssetRegistry::GetDefaultMaterial()
{
	return myMaterials["default"];
}
