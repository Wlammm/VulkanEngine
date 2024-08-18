#pragma once
#include "Vulkan/VulkanDescriptorSet.h"
// GPU Driven Rendering Pipeline.

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

    void ExecuteComputePass(vk::CommandBuffer inCommandBuffer, const ComputePassResources& inComputePassResources);

    void EnsureCorrectBufferSizes();
    
    void CreateBuffers();

    void CreatePrePassResources();
    void CreateCullPassResources();
    void CreateDrawPassResources();

    void BuildFrameBuffer() const;
    void BuildPointLightBuffer();
    void BuildDirectionalLightBuffer() const;
    
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

    // FrameDescriptorSet.
    struct alignas(16) PointLightData
    {
        int myNumLights;
        struct alignas(16)
        {
            glm::vec4 myColor = { 0, 0, 0, 0 };
            glm::vec3 myPosition = { 0, 0, 0 };
            float myRange = 0;
        } myLights[10];
    } myPointLightData;
    VulkanBuffer* myPointLightBuffer;
};
