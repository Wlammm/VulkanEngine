#pragma once

class IndexBuffer;
class VertexBuffer;

class Mesh
{
public:
    VertexBuffer* GetVertexBuffer() const;
    IndexBuffer* GetIndexBuffer() const;
    const glm::vec4& GetSphereBounds() const;

    uint GetHandle() const;
    
private:
    VertexBuffer* myVertexBuffer;
    IndexBuffer* myIndexBuffer;
    glm::vec4 mySphereBounds;

    friend class MeshSystem;
    uint myHandle;
};
