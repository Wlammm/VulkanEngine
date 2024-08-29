#pragma once
#include "System/System.h"

class ResizableBuffer;
class IndexBuffer;
class VulkanBuffer;

class IndexBufferSystem : public System
{
public:
    IndexBufferSystem();
    ~IndexBufferSystem();

    IndexBuffer* UploadIndexBuffer(const List<uint>& inIndices);
    IndexBuffer* UploadIndexBuffer(VulkanBuffer* inStagingBuffer, const uint inVertexCount);
    
    void RemoveIndexBuffer(const IndexBuffer* inBuffer);
    const ResizableBuffer* GetGlobalIndexBuffer() const;

private:
    uint myUsedBufferSize = 0;
    uint myCurrentIndexOffset = 0;

    List<IndexBuffer*> myIndexBuffers;

    ResizableBuffer* myBuffer = nullptr;
};
