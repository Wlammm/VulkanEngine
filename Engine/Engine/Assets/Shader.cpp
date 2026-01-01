#include "EnginePch.h"
#include "Shader.h"

#include <shaderc/shaderc.hpp>
#include <dxc/dxcapi.h>

#include "Engine/Engine.h"
#include "Engine/Rendering/DXCompiler.h"
#include "Engine/Vulkan/HlslShaderIncluder.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanShaderIncluder.h"
#include "Spirv-Reflect/spirv_reflect.h"

vk::ShaderStageFlagBits GetStageFromModule(const SpvReflectShaderModule& module)
{
    switch (module.spirv_execution_model)
    {
    case SpvExecutionModelVertex:
        return vk::ShaderStageFlagBits::eVertex;
    case SpvExecutionModelFragment:
        return vk::ShaderStageFlagBits::eFragment;
    case SpvExecutionModelGLCompute:
        return vk::ShaderStageFlagBits::eCompute;
    case SpvExecutionModelTaskEXT:
        return vk::ShaderStageFlagBits::eTaskEXT;
    case SpvExecutionModelMeshEXT:
        return vk::ShaderStageFlagBits::eMeshEXT;
    default:
        return vk::ShaderStageFlagBits::eAll;
    }
}

bool IsHlslShader(const SourcePath& inSourcePath)
{
    return inSourcePath.extension() == ".hlsl";
}

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

Shader::~Shader()
{
    if (IsValid())
        VulkanContext::GetDevice()->destroyShaderModule(myShaderModule);
    
    RemoveFilewatcherCallbacks();
}

void Shader::LoadPropertiesFromSource()
{
    Asset::LoadPropertiesFromSource();
    
    check(std::filesystem::exists(GetSourcePath()));
    
    std::ifstream stream(GetSourcePath());
    std::stringstream ss;
    ss << stream.rdbuf();
    stream.close();
    std::string shaderSource = ss.str();
    

    const bool isHlslShader = IsHlslShader(GetSourcePath());
    
    if (isHlslShader)
    {
        CompileHlslToSpv(shaderSource);
    }
    else
    {
        CompileGlslToSpv(shaderSource);
    }
    
    GenerateReflectionInfo();
}

void Shader::PostPropertiesSerialized()
{
    Asset::PostPropertiesSerialized();
    
    // Recreate filewatcher callbacks incase we have new includes.
    RemoveFilewatcherCallbacks();
    CreateFilewatcherCallbacks(myIncludes);

    InitFromBinary(myShaderBinary);

    OnShaderRecompiled();
}

bool Shader::IsCacheValid() const
{
    // Check if any included files has been modified as that forces us to recompile the shader also.
    for (const IncludeData& includeFile : myIncludes)
    {
        std::filesystem::file_time_type sourceWriteTime = std::filesystem::last_write_time(includeFile.myIncludePath);
        
        if (includeFile.myLastWriteTime != sourceWriteTime)
            return false;
    }
    
    return Asset::IsCacheValid();
}

void Shader::Recompile()
{
    LoadPropertiesFromSource();
    PostPropertiesSerialized();
}

vk::ShaderModule Shader::GetAPIResource() const
{
    return myShaderModule;
}

const List<DescriptorSetInfo>& Shader::GetDescriptorSetInfos() const
{
    return myDescriptorSets;
}

void Shader::CompileGlslToSpv(const std::string& inShaderSource)
{
    myEntryPoint = "main";
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_4);
    options.SetWarningsAsErrors();
    options.SetGenerateDebugInfo();
    options.SetTargetSpirv(shaderc_spirv_version_1_5);
    options.SetOptimizationLevel(shaderc_optimization_level_performance);

    // Create the unique ptr and grab a raw pointer from it before passing it to the compiler so we can grab include data afterwards.
    std::unique_ptr<VulkanShaderIncluder> uniqueIncluder = std::make_unique<VulkanShaderIncluder>();
    VulkanShaderIncluder* includer = uniqueIncluder.get();
    options.SetIncluder(std::move(uniqueIncluder));
	
    shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(inShaderSource, GetKindFromExtension(GetSourcePath().extension().string()), GetSourcePath().filename().string().c_str(), options);

    if (result.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        LOG_ERROR("SHADER COMPILE ERROR [%s]: ", GetSourcePath().filename().string().c_str());
        LOG_ERROR("%s", result.GetErrorMessage().c_str());

        // Throw error if we dont already have a valid shader module from previously.
        check(myShaderModule);
        return;
    }
    
    // TODO: Maybe we should add support for iterator initialization to lists so we dont have to do this extra copy.
    myShaderBinary = std::vector<uint32_t>{result.cbegin(), result.cend() };
    
    myIncludes.Clear();
    for (const SourcePath& includePath : includer->GetIncludedFiles())
    {
        myIncludes.Add({includePath, std::filesystem::last_write_time(includePath) });
    }
}

bool IsHlslVertexShader(const std::string& inShaderSource)
{
    // TODO: This is a really slow way of finding out the shader type but works for now.
    return inShaderSource.contains("VSMain");
}

bool IsHlslFragmentShader(const std::string& inShaderSource)
{
    return inShaderSource.contains("PSMain");
}

void Shader::CompileHlslToSpv(const std::string& inShaderSource)
{
    // Create source buffer
    DxcBuffer sourceBuffer{};
    sourceBuffer.Ptr = inShaderSource.data();
    sourceBuffer.Size = inShaderSource.size();
    sourceBuffer.Encoding = DXC_CP_UTF8;
    
    std::wstring profile;
    if (IsHlslVertexShader(inShaderSource))
    {
        myEntryPoint = "VSMain";
        profile = L"vs_6_6";
    }
    else if (IsHlslFragmentShader(inShaderSource))
    {
        myEntryPoint = "PSMain";
        profile = L"ps_6_6";
    }
    else
        check(false && "No valid entry point found.");
    
    
    std::wstring wideEntryPointString = String::ToWString(myEntryPoint);
    
    std::vector<LPCWSTR> args = {
        L"-spirv",
        L"-fspv-target-env=vulkan1.3",
        L"-E", wideEntryPointString.c_str(),
        L"-T", profile.c_str(),
        L"-O3",
        L"-HV",
        L"2021",
        L"-WX",
#if DEBUG
        L"-Zi",
        L"-Qembed_debug",
#endif
    };

    ComPtr<IDxcUtils> utils = DXCompiler::gDxcUtils;

    std::filesystem::path shaderDir = GetSourcePath().parent_path();
    ComPtr<IDxcIncludeHandler> includeHandler = new HlslShaderIncluder(utils.Get(), shaderDir);
    
    ComPtr<IDxcResult> result;
    HRESULT hr = DXCompiler::gDxcCompiler->Compile(
        &sourceBuffer,
        args.data(),
        (uint32_t)args.size(),
        includeHandler.Get(),
        IID_PPV_ARGS(&result)
    );
    
    check(SUCCEEDED(hr));
    
    myIncludes.Clear();
    for (const std::filesystem::path& path : static_cast<HlslShaderIncluder*>(includeHandler.Get())->GetIncludedFiles())
    {
        myIncludes.Add({ path, std::filesystem::last_write_time(path) });
    }

    // Get errors
    ComPtr<IDxcBlobUtf8> errors;
    result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);

    if (errors && errors->GetStringLength() > 0)
    {
        LOG_ERROR("Failed to compile hlsl shader [%s]: %s", GetSourcePath().string().c_str(), errors->GetStringPointer());
        check(false);
    }

    // Get SPIR-V
    ComPtr<IDxcBlob> spirvBlob;
    result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&spirvBlob), nullptr);

    check(spirvBlob && "Failed to generate spv output from shader.");

    myShaderBinary.Resize(static_cast<ListSizeType>(spirvBlob->GetBufferSize() / sizeof(uint32_t)));
    memcpy(myShaderBinary.data(), spirvBlob->GetBufferPointer(), spirvBlob->GetBufferSize());
}

void Shader::CreateFilewatcherCallbacks(const List<IncludeData>& inIncludePaths)
{
    myCallbackHandle = Engine::GetFilewatcher().InsertWatch_TS(GetSourcePath(), std::bind(&Shader::Recompile, this));

    for(IncludeData& includeData : inIncludePaths)
    {
        includeData.myCallbackHandle = Engine::GetFilewatcher().InsertWatch_TS(includeData.myIncludePath, std::bind(&Shader::Recompile, this));
    }
}

void Shader::RemoveFilewatcherCallbacks()
{
    Engine::GetFilewatcher().RemoveWatch_TS(GetSourcePath(), myCallbackHandle);
    for(const IncludeData& data : myIncludes)
    {
        if (data.myCallbackHandle.IsValid())
            Engine::GetFilewatcher().RemoveWatch_TS(data.myIncludePath, data.myCallbackHandle);
    }
}

void Shader::GenerateReflectionInfo()
{
    myDescriptorSets.Clear();
    SpvReflectShaderModule module{};
    
    SpvReflectResult result = spvReflectCreateShaderModule(myShaderBinary.size() * sizeof(uint32_t), myShaderBinary.data(), &module);
    check(result == SPV_REFLECT_RESULT_SUCCESS && "Failed to generate reflection information for shader.");
    
    uint32_t setCount = 0;
    spvReflectEnumerateDescriptorSets(&module, &setCount, nullptr);
    
    List<SpvReflectDescriptorSet*> sets;
    sets.Resize(setCount);
    
    spvReflectEnumerateDescriptorSets(&module, &setCount, sets.data());
    
    for (SpvReflectDescriptorSet* set : sets)
    {
        DescriptorSetInfo& setInfo = myDescriptorSets.Emplace();
        setInfo.mySetIndex = set->set;
        
        for (uint32_t bindingIndex = 0; bindingIndex < set->binding_count; ++bindingIndex)
        {
            SpvReflectDescriptorBinding* binding = set->bindings[bindingIndex];
            DescriptorBindingInfo& bindingInfo = setInfo.myBindings.Emplace();
            
            bindingInfo.myBindingIndex = binding->binding;
            bindingInfo.myDescriptorType = static_cast<vk::DescriptorType>(binding->descriptor_type);
            bindingInfo.myShaderStageFlags = GetStageFromModule(module);
            bindingInfo.myName = binding->name;
            if (binding->type_description && binding->type_description->type_name)
                bindingInfo.myTypeName = binding->type_description->type_name;
        }
    }
}

void Shader::InitFromBinary(const List<uint32_t>& inData)
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
                                                           .setPObjectName(GetSourcePath().string().c_str())
                                                           .setObjectType(vk::ObjectType::eShaderModule));
#endif    
}
