#include "EnginePch.h"
#include "MeshSystem.h"

#include "IndexBuffer.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "Utils/MathUtils.hpp"
#include "Vulkan/ResizableBuffer.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"

MeshSystem::MeshSystem()
{
    myBuffer = new ResizableBuffer(
        VulkanAllocator::AllocateBuffer_TS(
            "MeshBuffer",
            vk::BufferCreateInfo()
            .setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc)
            .setSize(sizeof(MeshData) * 4),
            VMA_MEMORY_USAGE_AUTO));
}

MeshSystem::~MeshSystem()
{
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
}

Mesh* MeshSystem::UploadMesh_TS(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer, const glm::vec4& inBoundingSphere)
{
    Mesh* mesh = new Mesh();

    MeshUploadData uploadData{};
    uploadData.myVertexBuffer = inVertexBuffer;
    uploadData.myIndexBuffer = inIndexBuffer;
    uploadData.myBoundingSphere = inBoundingSphere;
    uploadData.myMesh = mesh;
    myQueuedMeshUploads.Add(uploadData);
    return mesh;
}

void MeshSystem::Tick()
{
    UploadAllQueuedMeshes();
}

ResizableBuffer* MeshSystem::GetBuffer() const
{
    return myBuffer;
}

void MeshSystem::UploadAllQueuedMeshes()
{
    myQueuedMeshUploads.Lock();

    for(MeshUploadData& uploadData : myQueuedMeshUploads)
    {
        UploadMesh(uploadData);
    }
    myQueuedMeshUploads.Clear();

    myQueuedMeshUploads.Unlock();
}

void MeshSystem::UploadMesh(const MeshUploadData& inUploadData)
{
    MeshData meshData{};
    meshData.myBoundingSphereModelSpace = inUploadData.myBoundingSphere;
    meshData.myIndexCount = inUploadData.myIndexBuffer->GetIndexCount();
    meshData.myIndexOffset = inUploadData.myIndexBuffer->GetOffset();
    check(meshData.myIndexOffset != (uint)-1);
    meshData.myVertexOffset = inUploadData.myVertexBuffer->GetOffset();
    meshData.myAlbedoIndex = 0;
    meshData.myNormalIndex = 0;
    meshData.myMaterialIndex = 0;
    
    myBuffer->SetData(&meshData, sizeof(MeshData), sizeof(MeshData) * myNumObjects);
    inUploadData.myMesh->myHandle = myNumObjects;
    myNumObjects++;
}
