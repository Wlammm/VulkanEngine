#pragma once
#include "IRenderPass.h"
#include "Engine/AssetRegistry/AssetDefines.h"
#include "Engine/Vulkan/VulkanDescriptorSet.h"

class Shader;

class ComputePass : public IRenderPass
{
public:
    ComputePass(const SourcePath& inShaderPath);
    ~ComputePass();

    virtual void SetupDescriptors() = 0;
    
    // Dispatch how many we want the shader to use. Everything else is already setup.
    virtual void DispatchCall(vk::CommandBuffer inCommandBuffer) = 0;
    
    void CreateResources() override;
    void DestroyResources() override;
    
    void Execute(vk::CommandBuffer inCommandBuffer) override;
    
    void OnShaderRecompiled();
    
protected:
    std::string myPassName = "";
    
    VulkanDescriptorSet myDescriptorSet;
    
    vk::PipelineLayout myPipelineLayout;
    vk::Pipeline myPipeline;
    
    SharedPtr<Shader> myShader;
};
