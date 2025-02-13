#include "EnginePch.h"
#include "GPUSceneSystem.h"

#include "Engine.h"
#include "ResizableBuffer.h"
#include "VulkanAllocator.h"
#include "Assets/Material.h"
#include "Assets/Texture.h"
#include "Components/StaticMeshComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "Rendering/Mesh.h"
#include "World/World.h"

#include "Shaders/MeshStructs.hpp"

GPUSceneSystem::GPUSceneSystem()
{
    mySparseBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS(
        "GPUScene Sparse Buffer",
        vk::BufferCreateInfo()
                .setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc)
                .setSize(sizeof(MeshInstanceData) * 4),
        VMA_MEMORY_USAGE_AUTO));

    myDenseBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS(
        "GPUScene Dense Buffer",
        vk::BufferCreateInfo()
                .setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc)
                .setSize(sizeof(uint) * 4),
        VMA_MEMORY_USAGE_AUTO));
}

GPUSceneSystem::~GPUSceneSystem()
{
    VulkanAllocator::DestroyBuffer_TS(mySparseBuffer);
    VulkanAllocator::DestroyBuffer_TS(myDenseBuffer);
}

void GPUSceneSystem::Tick()
{
    LOG("GPU Object Updates Last Frame: %i", myNumGPUObjectUpdatesThisFrame);
    myNumGPUObjectUpdatesThisFrame = 0;
}

MeshInstanceIndex GPUSceneSystem::AddMeshInstance(const MeshInstanceData& inMeshInstanceData)
{
    ZoneScoped;
    myNumGPUObjectUpdatesThisFrame++;
    MeshInstanceIndex instanceIndex = -1;
    if(!myFreeSparseIndices.IsEmpty())
    {
        instanceIndex = myFreeSparseIndices.Last();
        myFreeSparseIndices.RemoveLast();
    }
    else
    {
        instanceIndex = myNextFreeSparseIndex;
        myNextFreeSparseIndex++;
    }
    
    mySparseBuffer->SetData(&inMeshInstanceData, sizeof(MeshInstanceData), sizeof(MeshInstanceData) * instanceIndex);
    myDenseBufferCPURepresentation.Add(instanceIndex);
    
    myDenseBuffer->SetData(&instanceIndex, sizeof(uint), 4 + sizeof(uint) * (myDenseBufferCPURepresentation.size() - 1));

    // Update the num objects for the dense buffer.
    uint size = myDenseBufferCPURepresentation.size();
    myDenseBuffer->SetData(&size, sizeof(uint), 0);
    return instanceIndex;
}

void GPUSceneSystem::UpdateMeshInstance(const MeshInstanceIndex inMeshIndex, const MeshInstanceData& inMeshInstanceData)
{
    myNumGPUObjectUpdatesThisFrame++;
    mySparseBuffer->SetData(&inMeshInstanceData, sizeof(MeshInstanceData), sizeof(MeshInstanceData) * inMeshIndex);
}

const ResizableBuffer* GPUSceneSystem::GetSparseBuffer() const
{
    return mySparseBuffer;
}

const ResizableBuffer* GPUSceneSystem::GetDenseBuffer() const
{
    return myDenseBuffer;
}

uint GPUSceneSystem::GetNumObjects() const
{
    return myDenseBufferCPURepresentation.size();
}

void GPUSceneSystem::RemoveMeshInstance(const MeshInstanceIndex inMeshInstance)
{
    myNumGPUObjectUpdatesThisFrame++;
    const uint denseIndex = myDenseBufferCPURepresentation.FindIndex(inMeshInstance);
    myFreeSparseIndices.Add(inMeshInstance);

    uint lastIndex = myDenseBufferCPURepresentation.size() - 1;

    if(denseIndex != lastIndex)
    {
        // Move the last element into the slot of the one being removed
        myDenseBuffer->MoveData(4 + sizeof(uint) * lastIndex, 4 + sizeof(uint) * denseIndex, sizeof(uint));
        myDenseBufferCPURepresentation[denseIndex] = myDenseBufferCPURepresentation[lastIndex];
    }

    myDenseBufferCPURepresentation.RemoveLast();

    // Update the dense buffer with the new size
    uint size = myDenseBufferCPURepresentation.size();
    myDenseBuffer->SetData(&size, sizeof(uint), 0);
}
