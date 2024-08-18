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

    void Tick();
    
    static VertexBuffer* UploadVertexBuffer_TS(const List<Vertex>& inVertices);
    void RemoveVertexBuffer(const VertexBuffer* inBuffer);

    const VulkanBuffer* GetGlobalVertexBuffer() const;

private:
    void UploadAllQueuedVertexBuffers();
    void UploadVertexData(const List<Vertex>& inVertices, VertexBuffer* inBuffer);
    void GrowBuffer(const uint inRequiredSize);

private:
    uint myUsedBufferSize = 0;
    uint myCurrentVertexOffset = 0;


    struct VertexBufferUploadData
    {
        List<Vertex> myVertices;
        VertexBuffer* myBuffer;
    };
    inline static MutexList<VertexBufferUploadData> myQueuedVertexBufferUploads{};
    
    List<VertexBuffer*> myVertexBuffers;

    VulkanBuffer* myBuffer = nullptr;
};
