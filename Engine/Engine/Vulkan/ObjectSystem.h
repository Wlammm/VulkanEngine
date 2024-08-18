#pragma once
#include "Subsystem/System.h"

class Mesh;
class ResizableBuffer;

class ObjectSystem final : public System
{
public:
    ObjectSystem();
    ~ObjectSystem();

    void AddObject(const glm::mat4& inTransform, const Mesh* inMesh);

    const ResizableBuffer* GetBuffer() const;
    uint GetNumObjects() const;
    
private:
    uint myNumObjects = 0;
    ResizableBuffer* myBuffer = nullptr;

};
