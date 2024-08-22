#pragma once
#include "Vertex.hpp"
#include "Subsystem/System.h"

class VertexBuffer;
class VulkanBuffer;

class VertexBufferSystem : public System
{
public:
    VertexBufferSystem();
    ~VertexBufferSystem();

    VertexBuffer* UploadVertexBuffer(const List<Vertex>& inVertices);
    void RemoveVertexBuffer(const VertexBuffer* inBuffer);

    const VulkanBuffer* GetGlobalVertexBuffer() const;

private:
    void GrowBuffer(const uint inRequiredSize);

private:
    uint myUsedBufferSize = 0;
    uint myCurrentVertexOffset = 0;

    List<VertexBuffer*> myVertexBuffers;

    VulkanBuffer* myBuffer = nullptr;
};
