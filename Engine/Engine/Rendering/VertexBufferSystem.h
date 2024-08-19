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

    // SharedPtr is used here to not have to copy the vertex data while the mutexlist's lock is acquired. This should lower the lock wait for all threads.
    inline static MutexList<std::shared_ptr<VertexBufferUploadData>> myQueuedVertexBufferUploads{};
    
    List<VertexBuffer*> myVertexBuffers;

    VulkanBuffer* myBuffer = nullptr;
};
