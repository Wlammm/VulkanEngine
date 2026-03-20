#include "EnginePch.h"
#include "VertexBufferSystem.h"

#include "GPUResourceManager.h"
#include "VertexBufferHandle.h"
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

    GPUResourceManager::Get()->RegisterBuffer<VertexBufferData>(myBuffer->GetSparseBuffer());
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

    const uint byteSize = inVertexCount * sizeof(Vertex);
    const auto handle   = myBuffer->AllocateFromStagingBuffer(inStagingBuffer, byteSize);

    VertexBufferHandle* bufferHandle = new VertexBufferHandle();
    bufferHandle->myIndex = handle.mySparseIndex;
    myVertexBuffers.Add(bufferHandle);
    return bufferHandle;
}

void VertexBufferSystem::RemoveVertexBuffer(const VertexBufferHandle* inBuffer)
{
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
    myBuffer->Defrag(inMaxMoves);
}