#pragma once
#include "Engine/Delegates/MulticastDelegate.hpp"

class VulkanBuffer;

class IGPUBuffer
{
public:
    virtual ~IGPUBuffer() = default;
    virtual VulkanBuffer* GetBuffer() const = 0;
    virtual MulticastDelegate<void()>* GetOnBufferResized() const = 0; // This can return nullptr. But I want it to be explicit.
};
