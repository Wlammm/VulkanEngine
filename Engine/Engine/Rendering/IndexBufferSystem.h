#pragma once
#include "Subsystem/System.h"

class IndexBuffer;
class VulkanBuffer;

class IndexBufferSystem : public System
{
public:
    IndexBufferSystem();
    ~IndexBufferSystem();

    IndexBuffer* UploadIndexBuffer(const List<uint>& inIndices);
    
    void RemoveIndexBuffer(const IndexBuffer* inBuffer);
    const VulkanBuffer* GetGlobalIndexBuffer() const;

private:
    void GrowBuffer(const uint inRequiredSize);

private:
    uint myUsedBufferSize = 0;
    uint myCurrentIndexOffset = 0;

    List<IndexBuffer*> myIndexBuffers;

    VulkanBuffer* myBuffer = nullptr;
};
