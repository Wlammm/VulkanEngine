#pragma once
#include "Model.h"

class AssetRegistry
{
public:
	AssetRegistry();
	~AssetRegistry();

	void Init();

	class Model* GetModel(const std::filesystem::path& inPath, const Model::CreateInfo& inCreateInfo = Model::CreateInfo());
	class VulkanShader* GetShader(const std::filesystem::path& inPath);
	class Material* GetMaterial(const std::filesystem::path& inPath);
	class Material* GetDefaultMaterial();

private:
	std::unordered_map<std::filesystem::path, class Model*> myModels{};
	std::unordered_map<std::filesystem::path, class VulkanShader*> myShaders{};
	std::unordered_map<std::filesystem::path, class Material*> myMaterials{};
};