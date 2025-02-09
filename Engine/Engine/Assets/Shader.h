#pragma once
#include "AssetRegistry/Asset.h"
#include "Core/Filewatcher.h"
#include "Delegates/MulticastDelegate.hpp"

class VulkanShader;
class Shader : public Asset
{
public:
    Coroutine<void, void, false> Load(const std::filesystem::path inPath) override;
    void Unload() override;

    void Compile();

    vk::ShaderModule GetAPIResource() const;

    MulticastDelegate<void()> OnShaderRecompiled;
    
private:
    void CreateFilewatcherCallbacks(const List<std::filesystem::path>& inIncludePaths);
    void RemoveFilewatcherCallbacks();

    void InitFromFile();
    void InitFromBinary(const std::vector<uint32_t>& inData);

    
private:
    vk::ShaderModule myShaderModule;

    Filewatcher::CallbackHandle myCallbackHandle;
    
    struct IncludeFileData
    {
        std::filesystem::path myPath;
        Filewatcher::CallbackHandle myCallbackHandle;
    };
    List<IncludeFileData> myIncludeFiles;
};
