#pragma once
#include "Shaders/MeshStructs.hpp"
#include "Subsystem/System.h"

class Mesh;
class IndexBuffer;
class VertexBuffer;
class ResizableBuffer;

class MeshSystem : public System
{
public:
    MeshSystem();
    ~MeshSystem();

    Mesh* UploadMesh(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer, const glm::vec4& inBoundingSphere);

    ResizableBuffer* GetBuffer() const;
    
private:
    List<Mesh*> myMeshes{};
    ResizableBuffer* myBuffer;
    uint myNumObjects = 0;
};
