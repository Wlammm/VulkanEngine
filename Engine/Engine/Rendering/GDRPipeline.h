#pragma once
#include "Engine/Vulkan/VulkanDescriptorSet.h"
// GPU Driven Rendering Pipeline.

class TextureCube;
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
    SharedPtr<Shader> myPrePassShader = nullptr;
    SharedPtr<Shader> myCullShader = nullptr;

    TextureCube* myCubemap = nullptr;

    ComputePassResources myPrePass;
    ComputePassResources myCullPass;
    
    ResizableBuffer* myIndirectCommandsBuffer = nullptr;
    ResizableBuffer* myIndirectCommandsBufferNoDepth = nullptr;
    VulkanBuffer* myCountBuffer = nullptr;
    VulkanBuffer* myCountNoDepthBuffer = nullptr;
    ResizableBuffer* myPerDrawDataBuffer = nullptr;
    ResizableBuffer* myPerDrawDataNoDepthBuffer = nullptr;
    
    // ==== Draw resources ====
    VulkanDescriptorSet myFrameDescriptorSet{};
    VulkanDescriptorSet myFrameNoDepthDescriptorSet{};
    vk::PipelineLayout myPipelineLayout;
    vk::Pipeline myPipeline;
    
    SharedPtr<Shader> myVertexShader;
    SharedPtr<Shader> myFragmentShader;

    // FrameDescriptorSet.
    struct FrameData
    {
        glm::mat4 myToView;
        glm::mat4 myProjection;
        glm::vec3 myCameraPosition;
        uint myCubemapIndex = (uint)-1;
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
