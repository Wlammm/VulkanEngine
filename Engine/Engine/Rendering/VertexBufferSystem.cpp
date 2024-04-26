#include "EnginePch.h"
#include "VertexBufferSystem.h"

#include "Engine.h"
#include "ECS/Systems/RenderSystem.h"
#include "Utils/MathUtils.hpp"

#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanContext.h"

VertexBufferSystem::VertexBufferSystem()
{
    SetDependencies<RenderSystem>();
}

void VertexBufferSystem::Tick()
{
    System::Tick();

        
}

VertexBufferHandle VertexBufferSystem::UploadVertexData(const List<Vertex>& myVertices)
{
    check(!myVertices.IsEmpty() && "Vertices should not be empty.");

    uint requiredSize = myUsedBufferSize + myVertices.size() * sizeof(Vertex);
    GrowBuffer(requiredSize);

    myBuffer->SetData();
    
    return ++myNextHandleID;
}

void VertexBufferSystem::GrowBuffer(const uint inRequiredSize)
{
    if(!myBuffer)
    {
        vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
            .setSize(MathUtils::UpperPowerOfTwo(inRequiredSize))
            .setUsage(vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst);
        
        myBuffer = VulkanAllocator::AllocateBuffer_TS("GlobalVertexBuffer", createInfo, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE);
        return;
    }
    
    if(inRequiredSize <= myBuffer->GetSize())
        return;

    const vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
            .setSize(MathUtils::UpperPowerOfTwo(inRequiredSize))
            .setUsage(vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst);
    VulkanBuffer* newBuffer = VulkanAllocator::AllocateBuffer_TS("GlobalVertexBuffer", createInfo, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE);

    VulkanBuffer* oldBuffer = myBuffer;
    RenderSystem::AddUploadCommand_TS(this, [oldBuffer, newBuffer](vk::CommandBuffer inCommandBuffer)
    {
        const vk::BufferCopy copy = vk::BufferCopy().setSize(oldBuffer->GetSize()).setSrcOffset(0).setDstOffset(0);
        inCommandBuffer.copyBuffer(oldBuffer->GetAPIResource(), newBuffer->GetAPIResource(), {copy});
    });
    VulkanAllocator::DestroyBuffer_TS(oldBuffer);
    myBuffer = newBuffer;
}
