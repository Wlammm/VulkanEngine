#pragma once
#include "Shaders/MeshStructs.hpp"
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
    IndexBufferHandle* UploadIndexBuffer(VulkanBuffer* inStagingBuffer, const uint inIndexCount);

    const IndexBufferData& GetIndexBufferDataFromIndexHandle(IndexBufferHandle* inHandle);
    
    void RemoveIndexBuffer(const IndexBufferHandle* inBuffer);
    const ResizableBuffer* GetGlobalIndexBuffer() const;
    const ResizableBuffer* GetGlobalSparseIndexDataBuffer() const;

private:
    uint myUsedBufferSize = 0;
    uint myCurrentIndexOffset = 0;

    List<IndexBufferHandle*> myIndexBuffers;

    ResizableBuffer* myBuffer = nullptr;
    
    // This buffer contains the offsets & sizes for different meshes. This is a sparse buffer.
    ResizableBuffer* mySparseIndexDataBuffer;
    List<IndexBufferData> mySparseIndexData_CPURepresentation;
    List<uint> myFreeSparseIndices;
};
