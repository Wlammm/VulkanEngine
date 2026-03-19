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

struct DescriptorBindingInfo
{
    META(SerializeField)
    uint32_t myBindingIndex;
    
    META(SerializeField)
    vk::DescriptorType myDescriptorType;
    
    META(SerializeField)
    vk::ShaderStageFlags myShaderStageFlags;
    
    META(SerializeField)
    std::string myName;

    META(SerializeField)
    std::string myTypeName;

    META(SerializeField)
    bool myIsReadOnly = true;
};

struct DescriptorSetInfo
{
    META(SerializeField)
    uint32_t mySetIndex;
    META(SerializeField)
    List<DescriptorBindingInfo> myBindings;
};


class VulkanShader;
class Shader : public Asset
{
public:
    ~Shader() override;
    
    static List<std::string> GetAssetExtensions() { return {".vert", ".frag", ".comp" }; }
    
    void LoadPropertiesFromSource() override;
    void PostPropertiesSerialized() override;

    static bool IsExternalAsset() { return true; };
    
    bool IsCacheValid() const override;
    
    void Recompile();

    vk::ShaderModule GetAPIResource() const;
    
    const std::string& GetEntryPoint() const { return myEntryPoint; }
    
    const List<DescriptorSetInfo>& GetDescriptorSetInfos() const;

    // Called whenever the shader gets hot reloaded due to the file being changed.
    MulticastDelegate<void()> OnShaderRecompiled;
    
private:
    void CompileGlslToSpv(const std::string& inShaderSource);
    void CompileHlslToSpv(const std::string& inShaderSource);
    
    void CreateFilewatcherCallbacks(const List<IncludeData>& inIncludePaths);
    void RemoveFilewatcherCallbacks();

    void GenerateReflectionInfo();
    
    void InitFromBinary(const List<uint32_t>& inData);

    
private:
    vk::ShaderModule myShaderModule;

    Filewatcher::CallbackHandle myCallbackHandle;
    
    META(SerializeField)
    std::string myEntryPoint;
    
    META(SerializeField)
    List<uint32_t> myShaderBinary;
    
    META(SerializeField)
    List<DescriptorSetInfo> myDescriptorSets;
    
    META(SerializeField)
    List<IncludeData> myIncludes;
};