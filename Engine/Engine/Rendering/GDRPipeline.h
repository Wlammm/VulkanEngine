#pragma once
#include "Vulkan/VulkanDescriptorSet.h"
// GPU Driven Rendering Pipeline.

class VulkanShader;

class GDRPipeline
{
public:
    GDRPipeline();
    ~GDRPipeline();

    void AddCommands(vk::CommandBuffer inCommandBuffer);

private:
    void CreateDescriptorSets();
    void CreatePipeline();
    
private:
    VulkanShader* myCullShader = nullptr;

    vk::Pipeline myPipeline;
    vk::PipelineLayout myPipelineLayout;

    VulkanDescriptorSet myDescriptorSet;
};
