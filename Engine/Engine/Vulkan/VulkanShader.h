#pragma once
#include "Core/Filewatcher.h"

class AssetObserver;

class VulkanShader
{
public:
	~VulkanShader();

	operator vk::ShaderModule();

	void AddObserver(AssetObserver* inObserver);
	void RemoveObserver(AssetObserver* inObserver);

private:
	friend class AssetRegistry;

	VulkanShader(const std::filesystem::path& inPath);
	void InitFromFile();

	void InitFromBinary(const std::vector<uint32_t>& inData);

	void Compile();

private:
	std::filesystem::path myPath;
	vk::ShaderModule myShaderModule;

	Filewatcher::CallbackHandle myFilewatcherHandle;

	// List of objects to call when this asset has been updated.
	List<AssetObserver*> myObservers;
};