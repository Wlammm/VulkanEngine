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
    
    void DrawCall(vk::CommandBuffer inCommandBuffer) override;
    
private:
    SharedPtr<Model> mySkyboxModel = nullptr;
    SharedPtr<Texture> mySkybox = nullptr;
};
