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

    VertexBuffer* UploadVertexBuffer(VulkanBuffer* inStagingBuffer, const uint inVertexCount);
    VertexBuffer* UploadVertexBuffer(const List<Vertex>& inVertices);
    void RemoveVertexBuffer(const VertexBuffer* inBuffer);

    const VulkanBuffer* GetGlobalVertexBuffer() const;

    void GrowBuffer(const uint inRequiredSize);

    uint GetUsedBufferSize() const;
    
private:
    uint myUsedBufferSize = 0;
    uint myCurrentVertexOffset = 0;

    List<VertexBuffer*> myVertexBuffers;

    VulkanBuffer* myBuffer = nullptr;
};
