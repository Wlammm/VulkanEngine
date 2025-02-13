#include "EnginePch.h"
#include "StagingBuffer.h"

#include "Vulkan/VulkanBuffer.h"

StagingBuffer::StagingBuffer(VulkanBuffer* inUnderlyingBuffer, const uint inSize, const uint inOffset)
{
    myUnderlyingBuffer = inUnderlyingBuffer;
    mySize = inSize;
    myOffset = inOffset;
}

void StagingBuffer::SetData(const void* inData, uint inSize)
{
    check(inSize <= mySize);
    check(myUnderlyingBuffer);
    check(myUnderlyingBuffer->IsMappable());
    
    myUnderlyingBuffer->SetData(inData, inSize, myOffset);
}

uint StagingBuffer::GetSize() const
{
    return mySize;
}

uint StagingBuffer::GetOffset() const
{
    return myOffset;
}

VulkanBuffer* StagingBuffer::GetUnderlyingBuffer() const
{
    return myUnderlyingBuffer;
}
