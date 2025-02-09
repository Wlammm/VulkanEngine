#include "EnginePch.h"
#include "Shader.h"

#include <shaderc/shaderc.hpp>

#include "Engine.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanShaderIncluder.h"

shaderc_shader_kind GetKindFromExtension(const std::string& inExtension)
{
    if (String::EqualsIgnoreCase(inExtension, ".vert"))
        return shaderc_vertex_shader;
    if (String::EqualsIgnoreCase(inExtension, ".frag"))
        return shaderc_fragment_shader;
    if(String::EqualsIgnoreCase(inExtension, ".comp"))
        return shaderc_compute_shader;

    check(false);
    return shaderc_vertex_shader;
}

Coroutine<void, void, false> Shader::Load(const std::filesystem::path inPath)
{
    ZoneScoped;
    std::filesystem::path path = "../Engine/Engine/Shaders/";
    path += inPath;
    myPath = path;
    if (!std::filesystem::exists(myPath))
    {
        LOG_ERROR("SHADER COMPILE ERROR [%s]: FILE DOES NOT EXISTS.", myPath.filename().string().c_str());
        co_return;
    }

    Compile();
}

void Shader::Unload()
{
    VulkanContext::GetDevice()->destroyShaderModule(myShaderModule);
}

void Shader::Compile()
{
    ZoneScoped;
    if (!std::filesystem::exists(myPath))
    {
        LOG_ERROR("SHADER COMPILE ERROR [%s]: FILE DOES NOT EXISTS.", myPath.filename().string().c_str());
        return;
    }

    std::ifstream stream(myPath);
    std::stringstream ss;
    ss << stream.rdbuf();
    stream.close();
    std::string shaderSource = ss.str();
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
    options.SetWarningsAsErrors();
    options.SetGenerateDebugInfo();
    options.SetTargetSpirv(shaderc_spirv_version_1_5);
    options.SetOptimizationLevel(shaderc_optimization_level_performance);

    // Create the unique ptr and grab a raw pointer from it before passing it to the compiler so we can grab include data afterwards.
    std::unique_ptr<VulkanShaderIncluder> uniqueIncluder = std::make_unique<VulkanShaderIncluder>();
    VulkanShaderIncluder* includer = uniqueIncluder.get();
    options.SetIncluder(std::move(uniqueIncluder));
	
    shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(shaderSource, GetKindFromExtension(myPath.extension().string()), myPath.filename().string().c_str(), options);

    if (result.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        LOG_ERROR("SHADER COMPILE ERROR [%s]: ", myPath.filename().string().c_str());
        LOG_ERROR("%s", result.GetErrorMessage().c_str());

        // Throw error if we dont already have a valid shader module from previously.
        check(myShaderModule);
        return;
    }

    // Recreate filewatcher callbacks incase we have new includes.
    RemoveFilewatcherCallbacks();
    CreateFilewatcherCallbacks(includer->GetIncludedFiles());

    std::vector<uint32_t> binary = { result.cbegin(), result.cend() };
    InitFromBinary(binary);

    OnShaderRecompiled();
}

vk::ShaderModule Shader::GetAPIResource() const
{
    return myShaderModule;
}

void Shader::CreateFilewatcherCallbacks(const List<std::filesystem::path>& inIncludePaths)
{
    myCallbackHandle = Engine::GetFilewatcher().InsertWatch_TS(myPath, std::bind(&Shader::Compile, this));

    for(const std::filesystem::path& path : inIncludePaths)
    {
        IncludeFileData& data = myIncludeFiles.Emplace();
        data.myPath = path;
        data.myCallbackHandle = Engine::GetFilewatcher().InsertWatch_TS(path, std::bind(&Shader::Compile, this));
    }
}

void Shader::RemoveFilewatcherCallbacks()
{
    Engine::GetFilewatcher().RemoveWatch_TS(myPath, myCallbackHandle);
    for(const IncludeFileData& data : myIncludeFiles)
    {
        Engine::GetFilewatcher().RemoveWatch_TS(data.myPath, data.myCallbackHandle);
    }
    myIncludeFiles.Clear();
}

void Shader::InitFromFile()
{
    ZoneScoped;
    const std::filesystem::path path = "CompiledShaders/" + myPath.string() + ".spv";
    check(std::filesystem::exists(path) && "Invalid path");

    std::ifstream stream(path, std::ios::binary);
    std::string data = { std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>() };
    stream.close();

    check(false);
}

void Shader::InitFromBinary(const std::vector<uint32_t>& inData)
{
    ZoneScoped;

    if(myShaderModule)
    {
        LOG_WARNING("VulkanShader waits for device idle. Fix");
        VulkanContext::GetDevice()->waitIdle();
        VulkanContext::GetDevice()->destroyShaderModule(myShaderModule);
    }
    myShaderModule = VulkanContext::GetDevice()->createShaderModule(vk::ShaderModuleCreateInfo()
                                                                    .setCodeSize(inData.size() * sizeof(uint32_t))
                                                                    .setPCode(inData.data()));
#if DEBUG
    VulkanContext::GetDevice()->setDebugUtilsObjectNameEXT(vk::DebugUtilsObjectNameInfoEXT()
                                                           .setObjectHandle(VulkanContext::GetVulkanHandle(myShaderModule))
                                                           .setPObjectName(myPath.string().c_str())
                                                           .setObjectType(vk::ObjectType::eShaderModule));
#endif    
}