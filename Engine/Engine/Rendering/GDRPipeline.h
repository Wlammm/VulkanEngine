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

    VulkanBuffer* GetCountBuffer() const;
    VulkanBuffer* GetIndirectBuffer() const;
    ResizableBuffer* GetPerDrawDataBuffer() const;

private:
    struct ComputePassResources
    {
        VulkanShader* myShader = nullptr;
        vk::Pipeline myPipeline;
        vk::PipelineLayout myPipelineLayout;
        VulkanDescriptorSet* myDescriptorSet = nullptr;

        void Destroy();
    };
    
    void ExecuteComputePass(vk::CommandBuffer inCommandBuffer, const ComputePassResources& inComputePassResources);

    void CreateBuffers();

    void CreatePrePassResources();
    void CreateCullPassResources();
    
private:
    VulkanShader* myPrePassShader = nullptr;
    VulkanShader* myCullShader = nullptr;

    ComputePassResources myPrePass;
    ComputePassResources myCullPass;

    ResizableBuffer* myIndirectCommandsBuffer = nullptr;
    VulkanBuffer* myCountBuffer = nullptr;
    ResizableBuffer* myPerDrawDataBuffer = nullptr;
};
