#pragma once
#include "System/System.h"

class ResizableBuffer;
class IndexBufferHandle;
class VulkanBuffer;

class IndexBufferSystem : public System
{
public:
    IndexBufferSystem();
    ~IndexBufferSystem();

    IndexBufferHandle* UploadIndexBuffer(const List<uint>& inIndices);
    IndexBufferHandle* UploadIndexBuffer(VulkanBuffer* inStagingBuffer, const uint inVertexCount);
    
    void RemoveIndexBuffer(const IndexBufferHandle* inBuffer);
    const ResizableBuffer* GetGlobalIndexBuffer() const;

private:
    uint myUsedBufferSize = 0;
    uint myCurrentIndexOffset = 0;

    List<IndexBufferHandle*> myIndexBuffers;

    ResizableBuffer* myBuffer = nullptr;
};
