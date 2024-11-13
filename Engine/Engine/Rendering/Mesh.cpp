#include "EnginePch.h"
#include "Mesh.h"

VertexBufferHandle* Mesh::GetVertexBuffer() const
{
    return myVertexBuffer;    
}

IndexBufferHandle* Mesh::GetIndexBuffer() const
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

const std::string& Mesh::GetAlbedoPath() const
{
    return myAlbedoPath;
}

const std::string& Mesh::GetNormalPath() const
{
    return myNormalPath;
}

const std::string& Mesh::GetMaterialPath() const
{
    return myMaterialPath;
}
