#pragma once
#include "Delegates/MulticastDelegate.hpp"

class VulkanBuffer;

class ResizableBuffer
{
public:
    ResizableBuffer(VulkanBuffer* inBuffer);
    ~ResizableBuffer();

    ResizableBuffer() = delete;
    ResizableBuffer(const ResizableBuffer&) = delete;
    void operator=(const ResizableBuffer&) = delete;
    
    void Resize(const size_t inNewSize);

    mutable MulticastDelegate<void()> OnBufferResized;

    VulkanBuffer* GetBuffer() const;
    
private:
    bool myHasActiveCopy = false;
    VulkanBuffer* myBuffer = nullptr;
};
