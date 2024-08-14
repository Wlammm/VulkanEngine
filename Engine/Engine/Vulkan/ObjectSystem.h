#pragma once
#include "VulkanDynamicBuffer.hpp"
#include "Rendering/MeshSystem.h"
#include "Shaders/MeshStructs.hpp"

class ResizableBuffer;
class ObjectSystem final : public System
{
public:
    ObjectSystem();
    ~ObjectSystem();

    void AddObject(const glm::mat4& inTransform, const MeshHandle inMesh);

    const ResizableBuffer* GetBuffer() const;
    uint GetNumObjects() const;
    
private:
    uint myNumObjects = 0;
    ResizableBuffer* myBuffer = nullptr;

};
