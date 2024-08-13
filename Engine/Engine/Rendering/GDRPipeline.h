#pragma once
#include "Vulkan/VulkanDescriptorSet.h"
// GPU Driven Rendering Pipeline.

class VulkanShader;
class VulkanBuffer;

class GDRPipeline
{
public:
    GDRPipeline();
    ~GDRPipeline();

    void AddCommands(vk::CommandBuffer inCommandBuffer);

private:
    void CreateDescriptorSets();
    void CreatePipeline();

    void CreateBuffers();
    
private:
    VulkanShader* myCullShader = nullptr;

    vk::Pipeline myPipeline;
    vk::PipelineLayout myPipelineLayout;

    VulkanDescriptorSet myDescriptorSet;

    ResizableBuffer* myIndirectCommandsBuffer = nullptr;
    VulkanBuffer* myCountBuffer = nullptr;
};
