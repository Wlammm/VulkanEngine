#pragma once
#include <mutex>

#include "Engine/Shaders/Shared/MeshStructs.hpp"
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
    
private:
    List<Mesh*> myMeshes{};
    ResizableBuffer* myBuffer;
    uint myNumObjects = 0;

    // Serializes UploadMesh. myNumObjects is the mesh handle == slot index; an unsynchronized
    // read-modify-write here lets two concurrent uploads take the same handle / overwrite the same
    // MeshData slot, which makes one mesh render with another mesh's geometry (e.g. the landscape
    // rendering as the slot-0 skybox sphere) while keeping its own material.
    std::mutex myUploadMutex;
};
