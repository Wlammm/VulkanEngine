#pragma once
#include "IRenderPass.h"
#include "Engine/AssetRegistry/AssetDefines.h"
#include "Engine/Vulkan/VulkanDescriptorSet.h"

class Shader;

class GraphicsPass : public IRenderPass
{
public:
    GraphicsPass(const SourcePath& inVertexShaderPath, const SourcePath& inFragmentShaderPath);
    ~GraphicsPass();
    
    virtual void SetupDescriptors() = 0;
    // Do the draw calls here.
    virtual void DrawCall(vk::CommandBuffer inCommandBuffer) = 0;
    
    void Execute(vk::CommandBuffer inCommandBuffer) override;
    
    void CreateResources() override;
    void DestroyResources() override;
    
    void OnShaderRecompiled();
    
protected:
    VulkanDescriptorSet myDescriptorSet;
    vk::PipelineLayout myPipelineLayout;
    vk::Pipeline myPipeline;
    
    SharedPtr<Shader> myVertexShader;
    SharedPtr<Shader> myFragmentShader;
};
