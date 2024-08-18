#include "EnginePch.h"
#include "VertexBuffer.h"

uint VertexBuffer::GetOffset() const
{
    return myOffset;
}

uint VertexBuffer::GetVertexCount() const
{
    return myVertexCount;
}