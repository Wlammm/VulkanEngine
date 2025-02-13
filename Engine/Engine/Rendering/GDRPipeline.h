#pragma once
#include "Vulkan/VulkanDescriptorSet.h"
// GPU Driven Rendering Pipeline.

class TransformComponent;
class Shader;
class VulkanBuffer;

class GDRPipeline
{
public:
    GDRPipeline();
    ~GDRPipeline();

    void AddComputeCommands(vk::CommandBuffer inCommandBuffer);
    void AddGraphicsCommands(vk::CommandBuffer inCommandBuffer);

    VulkanBuffer* GetCountBuffer() const;
    VulkanBuffer* GetIndirectBuffer() const;
    ResizableBuffer* GetPerDrawDataBuffer() const;

private:
    struct ComputePassResources
    {
        Shader* myShader = nullptr;
        vk::Pipeline myPipeline;
        vk::PipelineLayout myPipelineLayout;
        VulkanDescriptorSet* myDescriptorSet = nullptr;

        void Destroy();
    };

    void ExecuteComputePass(vk::CommandBuffer inCommandBuffer, const ComputePassResources& inComputePassResources, const glm::u32vec3& inGroupCount = glm::u32vec3(1, 1, 1));

    void EnsureCorrectBufferSizes(vk::CommandBuffer inCommandBuffer);
    
    void CreateBuffers();

    void CreatePrePassResources();
    void CreateCullPassResources();
    void CreateDrawPassResources();

    void OnShaderRecompiled();
    void CreateGraphicsPipeline();

    void BuildFrameBuffer() const;
    void BuildDirectionalLightBuffer() const;

    void OnTransformMarkedDirty(TransformComponent* inTransform);
    
private:
    Shader* myPrePassShader = nullptr;
    Shader* myCullShader = nullptr;

    ComputePassResources myPrePass;
    ComputePassResources myCullPass;
    
    ResizableBuffer* myIndirectCommandsBuffer = nullptr;
    VulkanBuffer* myCountBuffer = nullptr;
    ResizableBuffer* myPerDrawDataBuffer = nullptr;
    
    // ==== Draw resources ====
    VulkanDescriptorSet myFrameDescriptorSet{};
    vk::PipelineLayout myPipelineLayout;
    vk::Pipeline myPipeline;
    
    Shader* myVertexShader;
    Shader* myFragmentShader;

    // FrameDescriptorSet.
    struct FrameData
    {
        glm::mat4 myToView;
        glm::mat4 myProjection;
        glm::vec3 myCameraPosition;
    };
    VulkanBuffer* myFrameDataBuffer; 

    // FrameDescriptorSet.
    struct DirectionalLightBuffer
    {
        glm::vec4 myColor;
        glm::vec3 myDirection;
        float padding;
        glm::mat4 myLightView;
        glm::mat4 myLightProjection;
    };
    VulkanBuffer* myDirectionalLightBuffer;

    List<TransformComponent*> myDirtyTransforms;
};
