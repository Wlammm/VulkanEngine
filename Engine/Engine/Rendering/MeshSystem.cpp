#include "EnginePch.h"
#include "MeshSystem.h"

#include "GPUResourceManager.h"
#include "IndexBufferHandle.h"
#include "Mesh.h"
#include "VertexBufferHandle.h"
#include "Engine/Vulkan/ResizableBuffer.h"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"

MeshSystem::MeshSystem()
{
    myBuffer = new ResizableBuffer(
        VulkanAllocator::AllocateBuffer_TS(
            "MeshBuffer",
            vk::BufferCreateInfo()
            .setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc)
            .setSize(sizeof(MeshData) * 4),
            VMA_MEMORY_USAGE_AUTO));
    
    GPUResourceManager::Get()->RegisterBuffer<MeshData>(myBuffer);
}

MeshSystem::~MeshSystem()
{
}

Mesh* MeshSystem::UploadMesh(VertexBufferHandle* inVertexBuffer, IndexBufferHandle* inIndexBuffer, const glm::vec4& inBoundingSphere)
{
    ZoneScoped;
    Mesh* mesh = new Mesh();
    MeshData meshData{};
    meshData.myBoundingSphereModelSpace = inBoundingSphere;
    meshData.myIndexDataIndex = inIndexBuffer->GetIndex();
    check(meshData.myIndexDataIndex != (uint)-1);
    meshData.myVertexIndex = inVertexBuffer->GetIndex();
    check(meshData.myVertexIndex != (uint)-1);
    
    myBuffer->SetData(&meshData, sizeof(MeshData), sizeof(MeshData) * myNumObjects);
    mesh->myHandle = myNumObjects;
    mesh->myIndexBuffer = inIndexBuffer;
    mesh->myVertexBuffer = inVertexBuffer;
    myNumObjects++;
    myMeshes.Add(mesh);
    return mesh;
}

void MeshSystem::RemoveMesh(Mesh* inMesh)
{
    LOG_WARNING("MeshSystem::RemoveMesh not implemented!");
}