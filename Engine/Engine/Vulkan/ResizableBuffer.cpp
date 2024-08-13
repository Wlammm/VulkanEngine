#include "EnginePch.h"
#include "ResizableBuffer.h"

#include "Engine.h"
#include "VulkanAllocator.h"
#include "VulkanBuffer.h"
#include "Rendering/RenderSystem.h"

ResizableBuffer::ResizableBuffer(VulkanBuffer* inBuffer)
{
    myBuffer = inBuffer;
}

ResizableBuffer::~ResizableBuffer()
{
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
    myBuffer = nullptr;
}

void ResizableBuffer::Resize(const size_t inNewSize)
{
    check(myBuffer->GetCreateInfo().usage & vk::BufferUsageFlagBits::eTransferSrc &&
          myBuffer->GetCreateInfo().usage & vk::BufferUsageFlagBits::eTransferDst && "Buffers wasnt created with transfer src & dest usage flags.");

    check(!myHasActiveCopy && "ResizableBuffer cannot be resized multiple times during the same frame.");
    
    if(inNewSize < myBuffer->GetSize())
        return;

    VulkanBuffer* oldBuffer = myBuffer;
    
    vk::BufferCreateInfo createInfo = myBuffer->GetCreateInfo();
    createInfo.setSize(inNewSize);
    myBuffer = VulkanAllocator::AllocateBuffer_TS(myBuffer->GetName(), createInfo, myBuffer->GetVmaMemoryUsage(), myBuffer->IsMappable());
    myHasActiveCopy = true;

    Engine::GetEngineSystem<RenderSystem>().AddUploadCommand_TS(this, [this, oldBuffer](vk::CommandBuffer inCommandBuffer)
    {
        vk::BufferCopy copy = vk::BufferCopy().setSize(oldBuffer->GetSize());
        inCommandBuffer.copyBuffer(oldBuffer->GetAPIResource(), myBuffer->GetAPIResource(), copy);
        myHasActiveCopy = false;
    });
    
    VulkanAllocator::DestroyBuffer_TS(oldBuffer);
    OnBufferResized();
}

VulkanBuffer* ResizableBuffer::GetBuffer() const
{
    return myBuffer;
}