#pragma once
#include <mutex>

#include "Vertex.hpp"
#include "Engine/System/System.h"
#include "Engine/Shaders/Shared/MeshStructs.hpp"
#include "Engine/Vulkan/Containers/GPUDefragBuffer.h"

class VertexBufferHandle;
class VulkanBuffer;

/*
 *  This system manages all vertex data for the game in a single GPU buffer.
 *  Backed by a GPUDefragBuffer which provides stable handles, free-list allocation,
 *  and incremental defragmentation.
 */
class VertexBufferSystem : public System
{
public:
    VertexBufferSystem();
    ~VertexBufferSystem();

    VertexBufferHandle* UploadVertexBuffer(VulkanBuffer* inStagingBuffer, uint inVertexCount);
    void RemoveVertexBuffer(const VertexBufferHandle* inBuffer);

    uint GetVertexOffsetFromVertexHandle(VertexBufferHandle* inBuffer) const;

    void Defrag(uint inMaxMoves);

private:
    GPUDefragBuffer<VertexBufferData>* myBuffer = nullptr;
    List<VertexBufferHandle*> myVertexBuffers;

    // Serializes UploadVertexBuffer / RemoveVertexBuffer / Defrag — all mutate the same
    // GPUDefragBuffer free-lists, sparse entries and high-water mark. Unsynchronized access aliases
    // vertex allocations (same class of bug as MeshSystem::UploadMesh).
    std::mutex myUploadMutex;
};