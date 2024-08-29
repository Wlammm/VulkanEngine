#pragma once
#include "Delegates/MulticastDelegate.hpp"

class VulkanBuffer;

class ResizableBuffer
{
public:
    ResizableBuffer(VulkanBuffer* inBuffer);
    ResizableBuffer() = delete;
    ResizableBuffer(const ResizableBuffer&) = delete;
    void operator=(const ResizableBuffer&) = delete;

    mutable MulticastDelegate<void()> OnBufferResized;

    VulkanBuffer* GetBuffer() const;

    void CopyDataFromBuffer(VulkanBuffer* inStagingBuffer, const size_t inSize, uint inOffset);
    void SetData(const void* inData, const size_t inSize, uint inOffset = 0);

    template<typename T>
    void SetData(const T& inData)
    {
        static_assert(!std::is_pointer<T>::value && "Data type cannot be of pointer type");
        SetData(&inData, sizeof(T));
    }

    void DequeueUploads();

    void Resize(const size_t inRequiredSize);
    
private:
    friend class VulkanAllocator;
    ~ResizableBuffer() = default;
    
    bool myHasActiveUpload = false;
    bool myHasRegisteredForTick = false;
    VulkanBuffer* myBuffer = nullptr;
    size_t myCurrentSize = 0;

    struct UploadData
    {
        void* myCopiedData = nullptr;
        size_t mySize = 0;
        uint myOffset = 0;
    };
    List<UploadData> myDataToUpload{};

    struct StagedUploadData
    {
        VulkanBuffer* myBuffer = nullptr;
        size_t mySize = 0;
        uint myOffset = 0;
    };
    List<StagedUploadData> myStagedDataToUpload{};
};
