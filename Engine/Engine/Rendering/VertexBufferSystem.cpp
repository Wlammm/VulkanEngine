#include "EnginePch.h"
#include "VertexBufferSystem.h"

#include "GPUResourceManager.h"
#include "VertexBufferHandle.h"
#include "Engine/Utils/ThreadUtils.hpp"
#include "Engine/Vulkan/VulkanBuffer.h"

static_assert(sizeof(Vertex) == VERTEX_STRIDE_BYTES, "VERTEX_STRIDE_BYTES in MeshStructs.hpp is out of date. Update it to match sizeof(Vertex).");

VertexBufferSystem::VertexBufferSystem()
{
    const vk::BufferCreateInfo denseInfo = vk::BufferCreateInfo()
        .setSize(sizeof(Vertex) * 16)
        .setUsage(vk::BufferUsageFlagBits::eVertexBuffer
                | vk::BufferUsageFlagBits::eTransferDst
                | vk::BufferUsageFlagBits::eTransferSrc); // eTransferSrc required for defrag MoveData

    const vk::BufferCreateInfo sparseInfo = vk::BufferCreateInfo()
        .setSize(sizeof(VertexBufferData) * 16)
        .setUsage(vk::BufferUsageFlagBits::eStorageBuffer
                | vk::BufferUsageFlagBits::eTransferDst
                | vk::BufferUsageFlagBits::eTransferSrc);

    myBuffer = new GPUDefragBuffer<VertexBufferData>(
        denseInfo,  "Global Vertex Buffer",
        sparseInfo, "Global Sparse Vertex Data Buffer",
        VMA_MEMORY_USAGE_AUTO);

    // The sparse buffer is owned by the GPUDefragBuffer (destroyed in its destructor),
    // so register it non-owned here — otherwise GPUResourceManager would also delete it,
    // double-freeing it via ~GPUDefragBuffer() on shutdown.
    GPUResourceManager::Get()->RegisterBuffer<VertexBufferData>(myBuffer->GetSparseBuffer(), nullptr, false);
    GPUResourceManager::Get()->RegisterBuffer<Vertex>(myBuffer);
}

VertexBufferSystem::~VertexBufferSystem()
{
    for (VertexBufferHandle* handle : myVertexBuffers)
        del(handle);

    myVertexBuffers.Clear();

    //delete myBuffer;
    //myBuffer = nullptr;
}

VertexBufferHandle* VertexBufferSystem::UploadVertexBuffer(VulkanBuffer* inStagingBuffer, const uint inVertexCount)
{
    check(inVertexCount > 0);

    if (!ThreadUtils::IsOnMainThread())
        LOG_WARNING("VertexBufferSystem::UploadVertexBuffer called off the main thread. GPU uploads should run on the main thread.");

    std::scoped_lock lock(myUploadMutex);

    const uint byteSize = inVertexCount * sizeof(Vertex);
    const auto handle   = myBuffer->AllocateFromStagingBuffer(inStagingBuffer, byteSize);

    VertexBufferHandle* bufferHandle = new VertexBufferHandle();
    bufferHandle->myIndex = handle.mySparseIndex;
    myVertexBuffers.Add(bufferHandle);
    return bufferHandle;
}

void VertexBufferSystem::RemoveVertexBuffer(const VertexBufferHandle* inBuffer)
{
    std::scoped_lock lock(myUploadMutex);
    myBuffer->Deallocate({ inBuffer->myIndex });
    myVertexBuffers.Remove(const_cast<VertexBufferHandle*>(inBuffer));
    del(inBuffer);
}

uint VertexBufferSystem::GetVertexOffsetFromVertexHandle(VertexBufferHandle* inBuffer) const
{
    return myBuffer->GetSparseEntry({ inBuffer->myIndex }).myByteOffset / sizeof(Vertex);
}

void VertexBufferSystem::Defrag(const uint inMaxMoves)
{
    // Defrag moves dense allocations and rewrites sparse entries — the same state UploadVertexBuffer
    // mutates — so it must take the same lock to avoid aliasing with a concurrent upload.
    std::scoped_lock lock(myUploadMutex);
    myBuffer->Defrag(inMaxMoves);
}