#include "EnginePch.h"
#include "MeshSystem.h"

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
    del(myBuffer);
}

void MeshSystem::Tick()
{
    UploadQueuedMeshes();
}

MeshHandle MeshSystem::UploadMesh(const MeshData& inMesh)
{
    myQueuedMeshes.Add(inMesh);
    myNumObjects++;
    return myNumObjects - 1;
}

ResizableBuffer* MeshSystem::GetBuffer() const
{
    return myBuffer;
}

void MeshSystem::UploadQueuedMeshes()
{
    if(myQueuedMeshes.IsEmpty())
        return;
    
    if(myBuffer->GetBuffer()->GetSize() < sizeof(MeshData) * myNumObjects)
    {
        myBuffer->Resize(MathUtils::UpperPowerOfTwo(sizeof(MeshData) * myNumObjects));
    }
    
    myBuffer->GetBuffer()->SetData(myQueuedMeshes.data(), sizeof(MeshData) * myQueuedMeshes.size(), sizeof(MeshData) * myNumUploadedObjects);
    myNumUploadedObjects += myQueuedMeshes.size();
    myQueuedMeshes.Clear();
}
