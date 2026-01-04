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

    struct DebugVertex
    {
        glm::vec3 myPosition;
        Color myColor;
    };
};
