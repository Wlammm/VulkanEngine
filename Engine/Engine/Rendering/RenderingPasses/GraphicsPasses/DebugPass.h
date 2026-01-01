#pragma once
#include "Engine/Rendering/RenderingPasses/GraphicsPass.h"

class DebugPass : public GraphicsPass
{
public:
    DebugPass();
    ~DebugPass();    
    void SetupAttachments() override;
    void SetupDescriptors() override;
    
    List<vk::VertexInputBindingDescription>& GetVertexBindingDescriptor() const override;
    List<vk::VertexInputAttributeDescription>& GetVertexAttributeDescriptions() const override;
    vk::PrimitiveTopology GetPrimitiveTopology() const override;
    void DrawCall(vk::CommandBuffer inCommandBuffer) override;
    
private:
    VulkanBuffer* BuildVertexBuffer();
    void UpdateFrameBuffer();

    struct DebugVertex
    {
        glm::vec3 myPosition;
        Color myColor;
    };
    
    struct FrameData
    {
        glm::mat4 myToView;
        glm::mat4 myProjection;
        glm::vec3 myCameraPosition;
    };
    VulkanBuffer* myFrameDataBuffer;
};
