#pragma once
#include "Containers/IGPUBuffer.hpp"
#include "Engine/Delegates/MulticastDelegate.hpp"

class VulkanBuffer;

class ResizableBuffer : public IGPUBuffer
{
public:
    ResizableBuffer(VulkanBuffer* inBuffer);
    ResizableBuffer() = delete;
    ResizableBuffer(const ResizableBuffer&) = delete;
    void operator=(const ResizableBuffer&) = delete;

    VulkanBuffer* GetBuffer() const override;
    MulticastDelegate<void()>* GetOnBufferResized() const override;

    void CopyDataFromBuffer(VulkanBuffer* inStagingBuffer, const uint inSize, uint inOffset);
    void SetData(const void* inData, const uint inSize, uint inOffset = 0);

    // This function moves data from one part of the buffer to another.
    void MoveData(const uint inSourceOffset, const uint inDstOffset, const uint inSize);

    template<typename T>
    void SetData(const T& inData)
    {
        static_assert(!std::is_pointer<T>::value && "Data type cannot be of pointer type");
        SetData(&inData, sizeof(T));
    }

    void Resize(const uint inRequiredSize);

    // Ensures the buffer is bigger or equally big as inRequiredSize.
    void CheckCapacity(const uint inRequiredSize);
    
private:
    friend class VulkanAllocator;
    ~ResizableBuffer() = default;
    
    bool myHasActiveUpload = false;
    bool myHasRegisteredForTick = false;
    VulkanBuffer* myBuffer = nullptr;
    
    mutable MulticastDelegate<void()> OnBufferResized;
};
