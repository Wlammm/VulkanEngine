#include "EnginePch.h"
#include "ResizableBuffer.h"

#include "Engine.h"
#include "VulkanAllocator.h"
#include "VulkanBuffer.h"
#include "Rendering/RenderSystem.h"
#include "Utils/MathUtils.hpp"

ResizableBuffer::ResizableBuffer(VulkanBuffer* inBuffer)
{
    myBuffer = inBuffer;
}

ResizableBuffer::~ResizableBuffer()
{
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
    myBuffer = nullptr;
}

VulkanBuffer* ResizableBuffer::GetBuffer() const
{
    return myBuffer;
}

void ResizableBuffer::SetData(const void* inData, const size_t inSize, uint inOffset)
{
    UploadData& data = myDataToUpload.Emplace();
    data.myCopiedData = new char[inSize];
    memcpy(data.myCopiedData, inData, inSize);
    data.mySize = inSize;
    data.myOffset = inOffset;
    if(!myHasRegisteredForTick)
    {
        Engine::TickNextFrame.Bind(&ResizableBuffer::DequeueUploads, this);
        myHasRegisteredForTick = true;
    }
}

void ResizableBuffer::DequeueUploads()
{
    check(myBuffer->GetCreateInfo().usage & vk::BufferUsageFlagBits::eTransferSrc &&
          myBuffer->GetCreateInfo().usage & vk::BufferUsageFlagBits::eTransferDst && "Buffers wasnt created with transfer src & dest usage flags.");

    size_t requiredSize = myCurrentSize;
    for(const UploadData& uploadData : myDataToUpload)
    {
        requiredSize = std::max(requiredSize, uploadData.mySize + uploadData.myOffset);
    }

    if(myBuffer->GetSize() < requiredSize)
    {
        Resize(requiredSize);
    }
    
    for(UploadData& uploadData : myDataToUpload)
    {
        myBuffer->SetData(uploadData.myCopiedData, uploadData.mySize, uploadData.myOffset);
        del(uploadData.myCopiedData);
    }
    myDataToUpload.Clear();
    myHasRegisteredForTick = false;
}

void ResizableBuffer::Resize(const size_t inRequiredSize)
{
    check(!myHasActiveUpload && "You can only resize this buffer once per frame. This can happen if you call this method directly or via SetData. SetData can be called multiple times but you cant call SetData and Resize during the same frame.");
    
    // Resize buffer.
    size_t newSize = MathUtils::UpperPowerOfTwo(inRequiredSize);
    VulkanBuffer* oldBuffer = myBuffer;
    vk::BufferCreateInfo createInfo = myBuffer->GetCreateInfo();
    createInfo.setSize(newSize);
    myBuffer = VulkanAllocator::AllocateBuffer_TS(myBuffer->GetName(), createInfo, myBuffer->GetVmaMemoryUsage(), myBuffer->IsMappable());
    myHasActiveUpload = true;
    Engine::GetEngineSystem<RenderSystem>().AddUploadCommand_TS(this, [this, oldBuffer](vk::CommandBuffer inCommandBuffer)
    {
        myHasActiveUpload = false;
        vk::BufferCopy copy = vk::BufferCopy().setSize(oldBuffer->GetSize());
        inCommandBuffer.copyBuffer(oldBuffer->GetAPIResource(), myBuffer->GetAPIResource(), copy);
    });
    VulkanAllocator::DestroyBuffer_TS(oldBuffer);
    OnBufferResized();
}

/*
 *  ResizableBuffer->SetData(somePtr, 4, 4)
 *
 *
 */