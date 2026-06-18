#pragma once
#include <mutex>

#include "Engine/Shaders/Shared/MeshStructs.hpp"
#include "Engine/System/System.h"

class ResizableBuffer;
class IndexBufferHandle;
class VulkanBuffer;

using Index = uint;

class IndexBufferSystem : public System
{
public:
    IndexBufferSystem();
    ~IndexBufferSystem();

    IndexBufferHandle* UploadIndexBuffer(const List<uint>& inIndices);
    IndexBufferHandle* UploadIndexBuffer(VulkanBuffer* inStagingBuffer, const uint inIndexCount);

    const IndexBufferData& GetIndexBufferDataFromIndexHandle(IndexBufferHandle* inHandle);
    
    void RemoveIndexBuffer(const IndexBufferHandle* inBuffer);

private:
    uint myUsedBufferSize = 0;
    uint myCurrentIndexOffset = 0;

    List<IndexBufferHandle*> myIndexBuffers;

    ResizableBuffer* myBuffer = nullptr;
    
    // This buffer contains the offsets & sizes for different meshes. This is a sparse buffer.
    ResizableBuffer* mySparseIndexDataBuffer;
    List<IndexBufferData> mySparseIndexData_CPURepresentation;
    List<uint> myFreeSparseIndices;

    // Serializes both UploadIndexBuffer overloads: they share myUsedBufferSize / myCurrentIndexOffset
    // / the sparse table / the free list, and an unsynchronized read-modify-write there aliases index
    // ranges and sparse handles (same class of bug as MeshSystem::UploadMesh).
    std::mutex myUploadMutex;
};
