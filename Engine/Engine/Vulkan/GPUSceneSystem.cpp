#include "EnginePch.h"
#include "GPUSceneSystem.h"

#include "ResizableBuffer.h"
#include "VulkanAllocator.h"

#include "Engine/Shaders/Shared/MeshStructs.hpp"

GPUSceneSystem::GPUSceneSystem() 
    : myMeshes(VulkanBuffer::ResizableStorageBufferCreateInfo(sizeof(MeshInstanceData) * 16), "MeshBuffer", VMA_MEMORY_USAGE_AUTO, 16)
{ }

MeshInstanceIndex GPUSceneSystem::AddMeshInstance(const MeshInstanceData& inMeshInstanceData)
{
    ZoneScoped;
    return myMeshes.Add(inMeshInstanceData);
}

void GPUSceneSystem::UpdateMeshInstance(const MeshInstanceIndex inMeshIndex, const MeshInstanceData& inMeshInstanceData)
{
    ZoneScoped;
    myMeshes.Update(inMeshInstanceData, inMeshIndex);
}

void GPUSceneSystem::RemoveMeshInstance(const MeshInstanceIndex inMeshInstance)
{
    ZoneScoped;
    myMeshes.Remove(inMeshInstance);
}

uint GPUSceneSystem::GetNumObjects() const
{
    return myMeshes.Size();
}

const IGPUList* GPUSceneSystem::GetBuffer() const
{
    return &myMeshes;
}
