#pragma once
#include "Vertex.hpp"
#include "ECS/System.h"

class VulkanBuffer;

using VertexBufferHandle = uint;

struct VertexBufferData
{
    // The ID that identifies this vertex buffer. This ID will never be used again.
    VertexBufferHandle myID;

    // The offset into the global vertex buffer. This may change during defragmentation so do not store this anywhere else.
    uint myOffset;

    uint myVertexCount;
};

class VertexBufferSystem : public System
{
public:
    VertexBufferSystem();

    void Tick() override final;

    VertexBufferHandle UploadVertexData(const List<Vertex>& myVertices);

private:
    void GrowBuffer(const uint inRequiredSize);
    
private:
    uint myUsedBufferSize = 0;
    VertexBufferHandle myNextHandleID = 0;
    List<VertexBufferData> myVertexBuffers;

    VulkanBuffer* myBuffer = nullptr;
};
