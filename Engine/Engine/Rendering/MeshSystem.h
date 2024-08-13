#pragma once
#include "Shaders/MeshStructs.hpp"
#include "Subsystem/System.h"

class ResizableBuffer;
using MeshHandle = uint;

class MeshSystem : public System
{
public:
    MeshSystem();
    ~MeshSystem();

    void Tick();

    MeshHandle UploadMesh(const MeshData& inMesh);

    ResizableBuffer* GetBuffer() const;

private:
    void UploadQueuedMeshes();
    
private:
    List<MeshData> myQueuedMeshes{};
    ResizableBuffer* myBuffer;
    uint myNumObjects = 0;
    uint myNumUploadedObjects = 0;

    
};
