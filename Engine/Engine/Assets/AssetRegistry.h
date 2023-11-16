#pragma once
#include "Model.h"

class AssetRegistry
{
public:
	AssetRegistry();
	~AssetRegistry();

	class Model* GetModel(const std::filesystem::path& inPath, const Model::CreateInfo& inCreateInfo = Model::CreateInfo());

private:
	std::unordered_map<std::filesystem::path, class Model*> myModels{};
};