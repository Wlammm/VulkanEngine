#include "EnginePch.h"
#include "AssetRegistry.h"
#include "Model.h"
#include "Vulkan/VulkanShader.h"
#include "Material.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanImage.h"
#include "Factories/ModelFactory.h"
#include "tracy/Tracy.hpp"
#include "Factories/ImageFactory.h"

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
	del(myDefaultMaterial);
	myMaterials.clear();

	for (auto& [path, shader] : myShaders)
	{
		del(shader);
	}
	myShaders.clear();

	for (auto& [path, image] : myImages)
	{
		VulkanContext::GetAllocator().DestroyImage(image);
	}
	myImages.clear();
}

void AssetRegistry::Init()
{
	ScanDirectories();

	myDefaultMaterial = new Material();
}

Model* AssetRegistry::GetModel(const std::filesystem::path& inPath)
{
	ZoneScoped;
	if (!std::filesystem::exists(inPath))
	{
		LOG_WARNING("AssetRegistry::GetModel: Tried to load model with a filepath that doesnt exists. %s", inPath.string().c_str());
		return nullptr;
	}

	if(myModels.find(inPath) != myModels.end())
		return myModels[inPath];

	myModels[inPath] = ModelFactory::GetModel(inPath);
	return myModels[inPath];
}

VulkanShader* AssetRegistry::GetShader(const std::filesystem::path& inPath)
{
	ZoneScoped;
	if (myShaders.find(inPath) != myShaders.end())
		return myShaders[inPath];

	myShaders[inPath] = new VulkanShader(inPath);
	return myShaders[inPath];
}

Material* AssetRegistry::GetMaterial(const MaterialCreateInfo& inCreateInfo)
{
	ZoneScoped;
	if (myMaterials.find(inCreateInfo) == myMaterials.end())
		myMaterials[inCreateInfo] = new Material(inCreateInfo.myAlbedoPath, inCreateInfo.myNormalPath, inCreateInfo.myMaterialPath);
	return myMaterials[inCreateInfo];
}

Material* AssetRegistry::GetDefaultMaterial()
{
	return myDefaultMaterial;
}

VulkanImage* AssetRegistry::GetImage(const std::filesystem::path& inPath)
{
	ZoneScoped;
	if (myImages.find(inPath) == myImages.end())
		myImages[inPath] = ImageFactory::GetImage(inPath);

	return myImages[inPath];
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
