#pragma once
#include "Engine/Rendering/RenderingPasses/GraphicsPass.h"

class Model;
class Texture;

class SkyboxPass : public GraphicsPass
{
public:
    SkyboxPass();
    ~SkyboxPass();
    
    void SetupAttachments() override;
    void SetupDescriptors() override;
    
    void DrawCall(vk::CommandBuffer inCommandBuffer) override;
  
private:
    void BuildFrameData();
    
private:
    SharedPtr<Model> mySkyboxModel = nullptr;
    SharedPtr<Texture> mySkybox = nullptr;
    
    struct FrameData
    {
        glm::mat4 myToView;
        glm::mat4 myProjection;
        glm::vec3 myCameraPosition;
        uint myCubemapIndex = (uint)-1;
    };
    VulkanBuffer* myFrameDataBuffer;
};
