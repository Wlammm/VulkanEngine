#include "EnginePch.h"
#include "VertexBufferSystem.h"

#include "Engine.h"
#include "RenderSystem.h"
#include "VertexBuffer.h"
#include "Utils/MathUtils.hpp"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"

VertexBufferSystem::VertexBufferSystem()
{
    GrowBuffer(sizeof(Vertex) * 4);
}

VertexBufferSystem::~VertexBufferSystem()
{
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
    myUsedBufferSize = 0;

    for(VertexBuffer* vertexBuffer : myVertexBuffers)
    {
        del(vertexBuffer);
    }
    myVertexBuffers.Clear();
}

void VertexBufferSystem::Tick()
{
    UploadAllQueuedVertexBuffers();    
}

VertexBuffer* VertexBufferSystem::UploadVertexBuffer_TS(const List<Vertex>& inVertices)
{
    check(!inVertices.IsEmpty() && "Vertices should not be empty.");
    VertexBuffer* buffer = new VertexBuffer();
    std::shared_ptr<VertexBufferUploadData> uploadData = std::make_shared<VertexBufferUploadData>();
    uploadData->myVertices = inVertices;
    uploadData->myBuffer = buffer;
    myQueuedVertexBufferUploads.Add(uploadData);
    return buffer;
}

void VertexBufferSystem::UploadVertexData(const List<Vertex>& inVertices, VertexBuffer* inVertexBuffer)
{
    const uint sizeIncrease = inVertices.size() * sizeof(Vertex);
    const uint requiredSize = myUsedBufferSize + sizeIncrease;
    GrowBuffer(requiredSize);

    myBuffer->SetData(inVertices.data(), sizeIncrease, myUsedBufferSize);

    inVertexBuffer->myOffset = myCurrentVertexOffset;
    inVertexBuffer->myVertexCount = inVertices.size();
    myVertexBuffers.Add(inVertexBuffer);
    
    myUsedBufferSize += sizeIncrease;
    myCurrentVertexOffset += static_cast<uint>(inVertices.size());
}

void VertexBufferSystem::RemoveVertexBuffer(const VertexBuffer* inBuffer)
{
    LOG_WARNING("VertexBufferSubsystem::RemoveVertexBuffer not implemented.");
}

const VulkanBuffer* VertexBufferSystem::GetGlobalVertexBuffer() const
{
    return myBuffer;
}

void VertexBufferSystem::UploadAllQueuedVertexBuffers()
{
    ZoneScoped;
    myQueuedVertexBufferUploads.Lock();
    for(std::shared_ptr<VertexBufferUploadData> uploadData : myQueuedVertexBufferUploads)
    {
        UploadVertexData(uploadData->myVertices, uploadData->myBuffer);
    }
    myQueuedVertexBufferUploads.Clear();
    myQueuedVertexBufferUploads.Unlock();
}

void VertexBufferSystem::GrowBuffer(const uint inRequiredSize)
{
    ZoneScoped;
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
