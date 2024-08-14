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

MeshHandle MeshSystem::UploadMesh(const MeshData& inMesh)
{
    myBuffer->SetData(&inMesh, sizeof(MeshData), sizeof(MeshData) * myNumObjects);
    myNumObjects++;
    return myNumObjects - 1;
}

ResizableBuffer* MeshSystem::GetBuffer() const
{
    return myBuffer;
}
