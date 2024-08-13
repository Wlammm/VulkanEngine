#include "EnginePch.h"
#include "VertexBufferSystem.h"

#include "Engine.h"
#include "RenderSystem.h"
#include "Utils/MathUtils.hpp"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"

VertexBufferSystem::VertexBufferSystem()
{
}

VertexBufferSystem::~VertexBufferSystem()
{
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
    myUsedBufferSize = 0;
    myNextHandleID = 0;
    myVertexBuffers.clear();
}

VertexBufferHandle VertexBufferSystem::UploadVertexData(const List<Vertex>& myVertices)
{
    check(!myVertices.IsEmpty() && "Vertices should not be empty.");

    const uint sizeIncrease = myVertices.size() * sizeof(Vertex);
    const uint requiredSize = myUsedBufferSize + sizeIncrease;
    GrowBuffer(requiredSize);

    myBuffer->SetData(myVertices.data(), sizeIncrease, myUsedBufferSize);
    
    VertexBufferData data{};
    data.myOffset = myCurrentVertexOffset;
    data.myVertexCount = myVertices.size();
    data.myID = ++myNextHandleID;
    myVertexBuffers.insert({data.myID, data});
    
    myUsedBufferSize += sizeIncrease;
    myCurrentVertexOffset += static_cast<uint>(myVertices.size());

    myMaxVertexCount += myVertices.size();
    
    return data.myID;
}

void VertexBufferSystem::RemoveVertexBuffer(const VertexBufferHandle inHandle)
{
    LOG_WARNING("VertexBufferSubsystem::RemoveVertexBuffer not implemented.");
}

const VertexBufferData& VertexBufferSystem::GetVertexBufferData(const VertexBufferHandle inHandle) const
{
    check(myVertexBuffers.contains(inHandle) && "VertexBuffer handle is invalid.");
    return myVertexBuffers.at(inHandle);
}

const VulkanBuffer* VertexBufferSystem::GetGlobalVertexBuffer() const
{
    return myBuffer;
}

uint VertexBufferSystem::GetMaxVertexCount() const
{
    return myMaxVertexCount;
}

void VertexBufferSystem::GrowBuffer(const uint inRequiredSize)
{
    if(!myBuffer)
    {
        vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
            .setSize(MathUtils::UpperPowerOfTwo(inRequiredSize))
            .setUsage(vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc);
        
        myBuffer = VulkanAllocator::AllocateBuffer_TS("GlobalVertexBuffer", createInfo, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE);
        return;
    }
    
    if(inRequiredSize <= myBuffer->GetSize())
        return;

    const vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
            .setSize(MathUtils::UpperPowerOfTwo(inRequiredSize))
            .setUsage(vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc);
    VulkanBuffer* newBuffer = VulkanAllocator::AllocateBuffer_TS("GlobalVertexBuffer", createInfo, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE);

    VulkanBuffer* oldBuffer = myBuffer;
    Engine::GetEngineSystem<RenderSystem>().AddUploadCommand_TS(this, [oldBuffer, newBuffer](vk::CommandBuffer inCommandBuffer)
    {
        const vk::BufferCopy copy = vk::BufferCopy().setSize(oldBuffer->GetSize()).setSrcOffset(0).setDstOffset(0);
        inCommandBuffer.copyBuffer(oldBuffer->GetAPIResource(), newBuffer->GetAPIResource(), {copy});
    });
    VulkanAllocator::DestroyBuffer_TS(oldBuffer);
    myBuffer = newBuffer;
}
