#pragma once

class VulkanBuffer;

class StagingBuffer
{
public:
    StagingBuffer(VulkanBuffer* inUnderlyingBuffer, const uint inSize, const uint inOffset);

    void SetData(const void* inData, uint inSize);

    uint GetSize() const;
    uint GetOffset() const;
    VulkanBuffer* GetUnderlyingBuffer() const;
    
private:
    VulkanBuffer* myUnderlyingBuffer = nullptr;
    uint mySize = 0;
    uint myOffset = 0;
};