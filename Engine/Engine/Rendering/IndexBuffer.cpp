#include "EnginePch.h"
#include "IndexBuffer.h"

uint IndexBuffer::GetOffset() const
{
    return myOffset;
}
uint IndexBuffer::GetIndexCount() const
{
    return myIndexCount;
}