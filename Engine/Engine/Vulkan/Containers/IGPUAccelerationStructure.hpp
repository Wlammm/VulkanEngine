#pragma once
#include "Engine/Delegates/MulticastDelegate.hpp"

class IGPUAccelerationStructure
{
public:
    virtual ~IGPUAccelerationStructure() = default;
    virtual vk::AccelerationStructureKHR GetAccelerationStructure() const = 0;

    // Fired whenever the underlying VkAccelerationStructureKHR handle changes (e.g. on rebuild/resize).
    // Descriptor sets that reference this resource subscribe here and call Rebuild() in response.
    virtual MulticastDelegate<void()>* GetOnRebuilt() const = 0;
};