#include "EnginePch.h"
#include "AssetRegistry.h"
#include "Model.h"

AssetRegistry::AssetRegistry()
{
}

AssetRegistry::~AssetRegistry()
{
}

Model* AssetRegistry::GetModel(const std::filesystem::path& inPath)
{
	if (!std::filesystem::exists(inPath))
	{
		LOG_WARNING("AssetRegistry::GetModel: Tried to load model with a filepath that doesnt exists.");
		return nullptr;
	}

	if(myModels.find(inPath) != myModels.end())
		return myModels[inPath];

	myModels[inPath] = new Model(inPath, {});
	return myModels[inPath];
}
