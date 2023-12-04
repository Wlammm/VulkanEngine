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
	ScanDirectories();

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

const std::filesystem::path* AssetRegistry::TryGetPathFromFilename(const std::filesystem::path& inFilename) const
{
	std::filesystem::path filename = inFilename.filename();
	if (myFilenameToPath.find(filename) != myFilenameToPath.end())
		return &myFilenameToPath.at(filename);
	return nullptr;
}

const std::filesystem::path& AssetRegistry::GetPathFromFilename(const std::filesystem::path& inFilename) const
{
	std::filesystem::path filename = inFilename.filename();
	check(myFilenameToPath.find(filename) != myFilenameToPath.end());
	return myFilenameToPath.at(filename);
}

void AssetRegistry::ScanDirectories()
{
	int count = 0;
	for(const std::filesystem::path path : std::filesystem::recursive_directory_iterator("./"))
	{
		if (std::filesystem::is_directory(path))
			continue;

		count++;
		const FileName filename = path.filename();
		if(myFilenameToPath.find(path) != myFilenameToPath.end())
		{
			LOG_WARNING("[AssetRegistry]: Duplicate files with same name: %s", path.filename().string().c_str());
			continue;
		}
		myFilenameToPath.insert({ filename, path });
	}

	LOG("[AssetRegistry]: Directory scan complete. Scanned %i files.", count);
}
