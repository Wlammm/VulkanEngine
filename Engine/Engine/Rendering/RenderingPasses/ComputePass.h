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

    // Dispatch how many we want the shader to use. Everything else is already setup.
    virtual void DispatchCall(vk::CommandBuffer inCommandBuffer) = 0;
    
    void CreateResources() override;
    void DestroyResources() override;
    
    void Execute(vk::CommandBuffer inCommandBuffer) override;
    
    void OnShaderRecompiled();

    const char* GetName() const override { return myPassName.c_str(); }

protected:
    std::string myPassName = "";
    
    vk::PipelineLayout myPipelineLayout;
    vk::Pipeline myPipeline;
    
    SharedPtr<Shader> myShader;
};
