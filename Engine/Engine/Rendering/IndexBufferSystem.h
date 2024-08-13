#pragma once
#include "Subsystem/System.h"

class VulkanBuffer;
using IndexBufferHandle = uint;

struct IndexBufferData
{
    // The ID that identifies this vertex buffer. This ID will never be used again.
    IndexBufferHandle myID;

    // The offset into the global vertex buffer. This may change during defragmentation so do not store this anywhere else.
    uint myOffset;

    uint myIndexCount;
};

class IndexBufferSystem : public System
{
public:
    ~IndexBufferSystem();
    
    IndexBufferHandle UploadIndexData(const List<uint>& inIndices);
    void RemoveIndexBuffer(const IndexBufferHandle inHandle);
    
    const IndexBufferData& GetIndexBufferData(const IndexBufferHandle inHandle) const;
    const VulkanBuffer* GetGlobalIndexBuffer() const;

private:
    void GrowBuffer(const uint inRequiredSize);

private:
    uint myUsedBufferSize = 0;
    uint myCurrentIndexOffset = 0;
    IndexBufferHandle myNextHandleID = 0;
    
    std::unordered_map<IndexBufferHandle, IndexBufferData> myIndexBuffers;
    
    VulkanBuffer* myBuffer = nullptr;
};
