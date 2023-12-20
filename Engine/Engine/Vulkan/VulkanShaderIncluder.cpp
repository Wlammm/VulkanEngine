#include "EnginePch.h"
#include "VulkanShaderIncluder.h"
#include "Engine.h"
#include "Assets/AssetRegistry.h"

VulkanShaderIncluder::VulkanShaderIncluder()
{
}

VulkanShaderIncluder::~VulkanShaderIncluder()
{
    for(IncludeData& data : myActiveIncludes)
    {
        del(data.ptr);
    }
    myActiveIncludes.Clear();
}

shaderc_include_result* VulkanShaderIncluder::GetInclude(const char* inRequestedSource, shaderc_include_type inType, const char* inRequestingSource, size_t inIncludeDepth)
{
    static constexpr const char* shaderDirectory = "../Engine/Engine/Shaders/";
    std::filesystem::path path = shaderDirectory;
    path += inRequestedSource;

    shaderc_include_result* result = new shaderc_include_result();
    if (!std::filesystem::exists(path))
        return result;

    std::ifstream stream(path);
    std::stringstream contentStream;
    contentStream << stream.rdbuf();
    stream.close();

    IncludeData& includeData = myActiveIncludes.Emplace();
    includeData.sourceName = path.string();
    includeData.content = contentStream.str();
    includeData.ptr = result;

    result->source_name = includeData.sourceName.c_str();
    result->source_name_length = includeData.sourceName.length();
    result->content = includeData.content.c_str();
    result->content_length = includeData.content.length();

    return result;
}

void VulkanShaderIncluder::ReleaseInclude(shaderc_include_result* inData)
{
    // Do nothing here. Everything is handled in the destructor. 
    // This is so the include information is valid even after compilation.
}

List<std::filesystem::path> VulkanShaderIncluder::GetIncludedFiles() const
{
    List<std::filesystem::path> includes;

    for(const IncludeData& data : myActiveIncludes)
    {
        includes.Add(data.sourceName);
    }

    return includes;
}
