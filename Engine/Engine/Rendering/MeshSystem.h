#pragma once
#include "Engine/Rendering/SharedWithShaders/MeshStructs.hpp"
#include "Engine/System/System.h"

class Mesh;
class IndexBufferHandle;
class VertexBufferHandle;
class ResizableBuffer;

class MeshSystem : public System
{
public:
    MeshSystem();
    ~MeshSystem();

    Mesh* UploadMesh(VertexBufferHandle* inVertexBuffer, IndexBufferHandle* inIndexBuffer, const glm::vec4& inBoundingSphere);

    void RemoveMesh(Mesh* inMesh);
    
    ResizableBuffer* GetBuffer() const;
    
private:
    List<Mesh*> myMeshes{};
    ResizableBuffer* myBuffer;
    uint myNumObjects = 0;
};
