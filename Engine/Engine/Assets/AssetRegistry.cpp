#include "EnginePch.h"
#include "AssetRegistry.h"
#include "Model.h"

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
