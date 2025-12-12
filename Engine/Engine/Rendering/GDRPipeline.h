#pragma once
#include "Engine/Vulkan/VulkanDescriptorSet.h"
// GPU Driven Rendering Pipeline.

class IRenderPass;
class TextureCube;
class TransformComponent;
class Shader;
class VulkanBuffer;

class GDRPipeline
{
    inline static GDRPipeline* myInstance = nullptr;
public:
    static GDRPipeline* Get() { return myInstance;};
    GDRPipeline();
    ~GDRPipeline();

    template<typename RenderPassType>
    void AddRenderPass()
    {
        RenderPassType* renderPass = new RenderPassType();
        renderPass->CreateResources();
        myRenderPasses.Add(renderPass);
    }
    
    void AddComputeCommands(vk::CommandBuffer inCommandBuffer);
    void AddDepthPrepassCommands(vk::CommandBuffer inCommandBuffer);
    void AddGraphicsCommands(vk::CommandBuffer inCommandBuffer);

    VulkanBuffer* GetCountBuffer() const;
    VulkanBuffer* GetIndirectBuffer() const;
    ResizableBuffer* GetPerDrawDataBuffer() const;

    // TODO: THESE SHOULD NOT BE PUBLIC. This is just for testing rn.
    ResizableBuffer* myIndirectCommandsBuffer = nullptr;
    ResizableBuffer* myIndirectCommandsBufferNoDepth = nullptr;
    VulkanBuffer* myCountBuffer = nullptr;
    VulkanBuffer* myCountNoDepthBuffer = nullptr;
    ResizableBuffer* myPerDrawDataBuffer = nullptr;
    ResizableBuffer* myPerDrawDataNoDepthBuffer = nullptr;
    
    
private:
    void EnsureCorrectBufferSizes(vk::CommandBuffer inCommandBuffer);
    
    void CreateBuffers();

    void CreateDrawPassResources();

    void OnShaderRecompiled();
    void CreateGraphicsPipeline();

    void BuildFrameBuffer() const;
    void BuildDirectionalLightBuffer() const;

    void OnTransformMarkedDirty(TransformComponent* inTransform);
    
private:
    List<IRenderPass*> myRenderPasses;
    
    SharedPtr<Shader> myPrePassShader = nullptr;
    SharedPtr<Shader> myCullShader = nullptr;

    TextureCube* myCubemap = nullptr;

    
    
    
    // ==== Draw resources ====
    VulkanDescriptorSet myFrameDescriptorSet{};
    VulkanDescriptorSet myFrameNoDepthDescriptorSet{};
    vk::PipelineLayout myPipelineLayout;
    vk::Pipeline myPipeline;

    vk::PipelineLayout myPreDepthPipelineLayout;
    vk::Pipeline myPreDepthPipeline;
    
    
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
