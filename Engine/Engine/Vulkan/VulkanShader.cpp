#include "EnginePch.h"
#include "VulkanShader.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"

/*
*
* TODO:
* Reflect the data from the shader and create descriptor layouts in here
*
*/

VulkanShader::VulkanShader(const std::filesystem::path& inPath)
{
	myPath = inPath;
	//InitFromFile();
	Compile();
}

VulkanShader::operator vk::ShaderModule()
{
	return myShaderModule;
}

void VulkanShader::InitFromFile()
{
	const std::filesystem::path path = "CompiledShaders/" + myPath.string() + ".spv";
	check(std::filesystem::exists(path) && "Invalid path");

	std::ifstream stream(path, std::ios::binary);
	std::string data = { std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };
	stream.close();

	myShaderModule = VulkanContext::GetDevice()->createShaderModule(vk::ShaderModuleCreateInfo().setCodeSize(data.size()).setPCode((uint32_t*)data.data()));
}

void VulkanShader::Compile()
{
	std::filesystem::path path = "../Engine/Engine/Shaders/";
	path += myPath;
}
