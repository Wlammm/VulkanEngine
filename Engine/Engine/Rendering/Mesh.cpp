#include "EnginePch.h"
#include "Mesh.h"

VertexBuffer* Mesh::GetVertexBuffer() const
{
    return myVertexBuffer;    
}

IndexBuffer* Mesh::GetIndexBuffer() const
{
    return myIndexBuffer;
}

const glm::vec4& Mesh::GetSphereBounds() const
{
    return mySphereBounds;
}

uint Mesh::GetHandle() const
{
    return myHandle;
}