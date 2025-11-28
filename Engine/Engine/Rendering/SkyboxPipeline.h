#pragma once
#include "Engine/Vulkan/VulkanDescriptorSet.h"

class Texture;
class Model;
class TextureCube;
class Shader;

class SkyboxPipeline
{
public:
    SkyboxPipeline();
    ~SkyboxPipeline();
    void AddGraphicsCommands(vk::CommandBuffer inCommandBuffer);


private:
    void OnShaderRecompiled();

    void CreateDrawResources();
    void CreateGraphicsPipeline();

    void BuildFrameBuffer();
    
private:
    SharedPtr<Shader> myVertexShader = nullptr;
    SharedPtr<Shader> myFragmentShader = nullptr;

    // ==== Draw resources ====
    VulkanDescriptorSet myFrameDescriptorSet{};
    vk::PipelineLayout myPipelineLayout;
    vk::Pipeline myPipeline;

    // FrameDescriptorSet.
    struct FrameData
    {
        glm::mat4 myToView;
        glm::mat4 myProjection;
        glm::vec3 myCameraPosition;
        uint myCubemapIndex = (uint)-1;
    };
    VulkanBuffer* myFrameDataBuffer;

    SharedPtr<Model> mySkyboxModel = nullptr;
    SharedPtr<Texture> mySkybox = nullptr;
};
