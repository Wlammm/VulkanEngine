#include "EnginePch.h"
#include "IndexBufferHandle.h"

uint IndexBufferHandle::GetOffset() const
{
    return myOffset;
}
uint IndexBufferHandle::GetIndexCount() const
{
    return myIndexCount;
}