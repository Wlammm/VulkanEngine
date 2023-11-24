#pragma once
#include "Model.h"

class AssetRegistry
{
public:
	AssetRegistry();
	~AssetRegistry();

	class Model* GetModel(const std::filesystem::path& inPath, const Model::CreateInfo& inCreateInfo = Model::CreateInfo());
	class VulkanShader* GetShader(const std::filesystem::path& inPath);

private:
	std::unordered_map<std::filesystem::path, class Model*> myModels{};
	std::unordered_map<std::filesystem::path, class VulkanShader*> myShaders{};
};