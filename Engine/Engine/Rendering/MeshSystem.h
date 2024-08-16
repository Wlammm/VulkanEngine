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

    MeshHandle UploadMesh(const MeshData& inMesh);

    ResizableBuffer* GetBuffer() const;
    
private:
    ResizableBuffer* myBuffer;
    uint myNumObjects = 0;
};
