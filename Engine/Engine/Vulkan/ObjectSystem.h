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
    
    void Tick();

    const ResizableBuffer* GetBuffer() const;
    uint GetNumObjects() const;

private:
    void UploadQueuedObjects();
    
private:
    uint myNumObjects = 0;
    uint myNumUploadedObjects = 0;
    ResizableBuffer* myBuffer = nullptr;

    List<ObjectData> myQueuedObjects{};
    
    //VulkanDynamicBuffer<ObjectData> myBuffer{"RenderObject Buffer", vk::BufferCreateInfo().setUsage(vk::BufferUsageFlagBits::eStorageBuffer)};
};
