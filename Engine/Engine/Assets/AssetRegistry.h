#pragma once
#include "Model.h"

using FileName = std::filesystem::path;
using FullPath = std::filesystem::path;

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

	const std::filesystem::path* TryGetPathFromFilename(const std::filesystem::path& inFilename) const;
	const std::filesystem::path& GetPathFromFilename(const std::filesystem::path& inFilename) const;

private:
	void ScanDirectories();

private:
	std::unordered_map<std::filesystem::path, class Model*> myModels{};
	std::unordered_map<std::filesystem::path, class VulkanShader*> myShaders{};
	std::unordered_map<std::filesystem::path, class Material*> myMaterials{};

	std::unordered_map<FileName, FullPath> myFilenameToPath{};
};