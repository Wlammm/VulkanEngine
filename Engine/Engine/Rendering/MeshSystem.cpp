#include "EnginePch.h"
#include "MeshSystem.h"

#include "IndexBuffer.h"
#include "Mesh.h"
#include "VertexBuffer.h"
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

Mesh* MeshSystem::UploadMesh(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer, const glm::vec4& inBoundingSphere)
{
    ZoneScoped;
    Mesh* mesh = new Mesh();
    MeshData meshData{};
    meshData.myBoundingSphereModelSpace = inBoundingSphere;
    meshData.myIndexCount = inIndexBuffer->GetIndexCount();
    meshData.myIndexOffset = inIndexBuffer->GetOffset();
    check(meshData.myIndexOffset != (uint)-1);
    meshData.myVertexOffset = inVertexBuffer->GetOffset();
    check(meshData.myVertexOffset != (uint)-1);
    
    myBuffer->SetData(&meshData, sizeof(MeshData), sizeof(MeshData) * myNumObjects);
    mesh->myHandle = myNumObjects;
    mesh->myIndexBuffer = inIndexBuffer;
    mesh->myVertexBuffer = inVertexBuffer;
    myNumObjects++;
    myMeshes.Add(mesh);
    return mesh;
}

ResizableBuffer* MeshSystem::GetBuffer() const
{
    return myBuffer;
}