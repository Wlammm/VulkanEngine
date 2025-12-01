#pragma once
#include "Engine/AssetRegistry/Asset.h"
#include "Engine/Core/Filewatcher.h"
#include "Engine/Delegates/MulticastDelegate.hpp"

struct IncludeData
{
    META(SerializeField)
    SourcePath myIncludePath;
    
    META(SerializeField)
    std::filesystem::file_time_type myLastWriteTime;
    
    Filewatcher::CallbackHandle myCallbackHandle;
};

class VulkanShader;
class Shader : public Asset
{
public:
    ~Shader() override;
    
    List<std::string> GetAssetExtensions() const override { return {".vert", ".frag", ".comp" }; }
    
    void LoadPropertiesFromSource() override;
    void PostPropertiesSerialized() override;

    bool IsExternalAsset() const override { return true;};
    
    bool IsCacheValid() const override;
    
    void Recompile();

    vk::ShaderModule GetAPIResource() const;

    // Called whenever the shader gets hot reloaded due to the the file being changed.
    MulticastDelegate<void()> OnShaderRecompiled;
    
private:
    void CreateFilewatcherCallbacks(const List<IncludeData>& inIncludePaths);
    void RemoveFilewatcherCallbacks();

    void InitFromBinary(const List<uint32_t>& inData);

    
private:
    vk::ShaderModule myShaderModule;

    Filewatcher::CallbackHandle myCallbackHandle;
    
    META(SerializeField)
    List<uint32_t> myShaderBinary;
    
    META(SerializeField)
    List<IncludeData> myIncludes;
};
