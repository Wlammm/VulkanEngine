#pragma once
#include "System/System.h"

class Material;
class Mesh;
class ResizableBuffer;

class ObjectSystem final : public System
{
public:
    ObjectSystem();
    ~ObjectSystem();

    void AddObject(const glm::mat4& inTransform, const Mesh* inMesh, Material* inMaterial);

    const ResizableBuffer* GetBuffer() const;
    uint GetNumObjects() const;
    
private:
    uint myNumObjects = 0;
    ResizableBuffer* myBuffer = nullptr;

};
