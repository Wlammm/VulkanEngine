#pragma once
#include "ECS/System.h"
#include "Shaders/MeshStructs.hpp"
#include "Vulkan/VulkanDynamicBuffer.hpp"

class ResizableBuffer;
using MeshHandle = uint;

class MeshSystem final : public System
{
public:
    MeshSystem();
    ~MeshSystem();

    void Tick() override final;

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
