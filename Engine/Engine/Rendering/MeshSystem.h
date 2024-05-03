#pragma once
#include "ECS/System.h"
#include "Shaders/MeshStructs.hpp"
#include "Vulkan/VulkanDynamicBuffer.hpp"

using MeshHandle = uint;

class MeshSystem final : public System
{
public:
    MeshSystem();

    void Tick() override final;

    MeshHandle UploadRenderItem(const RenderItem& inRenderItem);
    void RemoveRenderItem(const MeshHandle inHandle);

    const VulkanDynamicBuffer<RenderItem>& GetBuffer() const;
    
private:
    VulkanDynamicBuffer<RenderItem> myRenderItems;
};
