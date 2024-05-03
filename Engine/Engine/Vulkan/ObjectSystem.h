#pragma once
#include "VulkanDynamicBuffer.hpp"
#include "ECS/System.h"
#include "Shaders/MeshStructs.hpp"

class ObjectSystem final : public System
{
public:
    ObjectSystem();

    void Tick() override final;

    const VulkanDynamicBuffer<RenderObject>& GetBuffer() const;

private:
    VulkanDynamicBuffer<RenderObject> myBuffer{"RenderObject Buffer", vk::BufferCreateInfo().setUsage(vk::BufferUsageFlagBits::eStorageBuffer)};
};
