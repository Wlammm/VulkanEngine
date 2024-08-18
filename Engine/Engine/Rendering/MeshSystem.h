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

    static Mesh* UploadMesh_TS(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer, const glm::vec4& inBoundingSphere);

    void Tick();
    
    ResizableBuffer* GetBuffer() const;
    
private:

    void UploadAllQueuedMeshes();
    
    struct MeshUploadData
    {
        VertexBuffer* myVertexBuffer;
        IndexBuffer* myIndexBuffer;
        glm::vec4 myBoundingSphere;
        Mesh* myMesh = nullptr;
    };
    inline static MutexList<MeshUploadData> myQueuedMeshUploads{};
    
    void UploadMesh(const MeshUploadData& inUploadData);
    
    ResizableBuffer* myBuffer;
    uint myNumObjects = 0;
};
