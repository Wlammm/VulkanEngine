#pragma once

class VulkanShader
{
public:

	operator vk::ShaderModule();

private:
	friend class AssetRegistry;

	VulkanShader(const std::filesystem::path& inPath);
	void InitFromFile();

	void Compile();

private:
	std::filesystem::path myPath;
	vk::ShaderModule myShaderModule;
};