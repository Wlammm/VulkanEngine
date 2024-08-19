#pragma once
#include "Subsystem/System.h"

class IndexBuffer;
class VulkanBuffer;

class IndexBufferSystem : public System
{
public:
    IndexBufferSystem();
    ~IndexBufferSystem();

    void Tick();

    static IndexBuffer* UploadIndexBuffer_TS(const List<uint>& inIndices);
    
    void RemoveIndexBuffer(const IndexBuffer* inBuffer);
    const VulkanBuffer* GetGlobalIndexBuffer() const;

private:
    void UploadAllQueuedIndexBuffers();
    void UploadIndexData(const List<uint>& inIndices, IndexBuffer* inBuffer);
    void GrowBuffer(const uint inRequiredSize);

private:
    uint myUsedBufferSize = 0;
    uint myCurrentIndexOffset = 0;

    List<IndexBuffer*> myIndexBuffers;

    struct IndexUploadData
    {
        IndexBuffer* myBuffer;
        List<uint> myIndices;
    };
    
    // SharedPtr is used here to not have to copy the vertex data while the mutexlist's lock is acquired. This should lower the lock wait for all threads.
    inline static MutexList<std::shared_ptr<IndexUploadData>> myQueuedUploadData{};
    
    VulkanBuffer* myBuffer = nullptr;
};
