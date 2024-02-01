#pragma once
#include "Model.h"
#include "Core/AutoInit.h"

using FileName = std::filesystem::path;
using FullPath = std::filesystem::path;

struct MaterialCreateInfo
{
	std::filesystem::path myAlbedoPath;
	std::filesystem::path myNormalPath;
	std::filesystem::path myMaterialPath;

	bool operator==(const MaterialCreateInfo& inOther) const
	{
		return myAlbedoPath == inOther.myAlbedoPath && myNormalPath == inOther.myNormalPath && myMaterialPath == inOther.myMaterialPath;
	}
};

template <>
struct std::hash<MaterialCreateInfo>
{
	std::size_t operator()(const MaterialCreateInfo& k) const
	{
		std::size_t res = 0;
		hash_combine(res, k.myAlbedoPath);
		hash_combine(res, k.myNormalPath);
		hash_combine(res, k.myMaterialPath);
		return res;
	}
};

class AssetRegistry : public AutoInit
{
public:
	AssetRegistry();
	~AssetRegistry();

	void Init() override final;

	class Model* GetModel(const std::filesystem::path& inPath);
	class VulkanShader* GetShader(const std::filesystem::path& inPath);
	class Material* GetMaterial(const MaterialCreateInfo& inCreateInfo);
	class Material* GetDefaultMaterial();
	class VulkanImage* GetImage(const std::filesystem::path& inPath);

	const std::filesystem::path* TryGetPathFromFilename(const std::filesystem::path& inFilename) const;
	const std::filesystem::path& GetPathFromFilename(const std::filesystem::path& inFilename) const;

private:
	void ScanDirectories();

private:
	std::unordered_map<std::filesystem::path, class Model*> myModels{};
	std::unordered_map<std::filesystem::path, class VulkanShader*> myShaders{};
	std::unordered_map<MaterialCreateInfo, class Material*> myMaterials{};
	class Material* myDefaultMaterial = nullptr;

	std::unordered_map<std::filesystem::path, class VulkanImage*> myImages{};

	std::unordered_map<FileName, FullPath> myFilenameToPath{};
};