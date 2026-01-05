#include "EnginePch.h"
#include "IndexBufferSystem.h"

#include "GPUResourceManager.h"
#include "Engine/Engine.h"
#include "IndexBufferHandle.h"
#include "RenderSystem.h"
#include "Engine/Utils/MathUtils.hpp"
#include "Engine/Vulkan/ResizableBuffer.h"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"


IndexBufferSystem::IndexBufferSystem()
{
    myBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("Global IndexBuffer", vk::BufferCreateInfo()
        .setSize(MathUtils::UpperPowerOfTwo(sizeof(uint) * 4))
        .setUsage(vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc), VMA_MEMORY_USAGE_AUTO, false));

    mySparseIndexDataBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("Global Sparse Index Data Buffer",
     vk::BufferCreateInfo()
     .setSize(sizeof(IndexBufferData) * 16)
     .setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst| vk::BufferUsageFlagBits::eTransferSrc)
     , VMA_MEMORY_USAGE_AUTO,
     false));
    
    GPUResourceManager::Get()->RegisterBuffer<IndexBufferData>(mySparseIndexDataBuffer);
    GPUResourceManager::Get()->RegisterBuffer<Index>(myBuffer);
}

IndexBufferSystem::~IndexBufferSystem()
{
    myUsedBufferSize = 0;
    myCurrentIndexOffset = 0;

    for(IndexBufferHandle* indexBuffer : myIndexBuffers)
    {
        del(indexBuffer);
    }
    myIndexBuffers.Clear();
    
    del(mySparseIndexDataBuffer);
    del(myBuffer);
}

IndexBufferHandle* IndexBufferSystem::UploadIndexBuffer(const List<uint>& inIndices)
{
    ZoneScoped;
    if(inIndices.IsEmpty())
    {
        LOG_WARNING("IndexBufferSystem::UploadIndexBuffer - Empty indices list");
        return nullptr;
    }

    // Upload the new data to the index data buffer.
    uint dataIndex = -1;
    if(!myFreeSparseIndices.IsEmpty())
    {
        dataIndex = myFreeSparseIndices.Last();
        myFreeSparseIndices.RemoveLast();
    }
    else
    {
        dataIndex = mySparseIndexData_CPURepresentation.size();
        mySparseIndexData_CPURepresentation.Emplace();
    }

    IndexBufferData& data = mySparseIndexData_CPURepresentation[dataIndex];
    data.myOffset = myCurrentIndexOffset;
    data.myCount = inIndices.size();
    
    IndexBufferHandle* buffer = new IndexBufferHandle();
    const uint sizeIncrease = inIndices.size() * sizeof(uint);
    myBuffer->SetData(inIndices.data(), sizeIncrease, myUsedBufferSize);
    mySparseIndexDataBuffer->SetData(&data, sizeof(IndexBufferData), sizeof(IndexBufferData) * dataIndex);
    
    buffer->myIndex = dataIndex;
    myIndexBuffers.Add(buffer);
    
    myUsedBufferSize += sizeIncrease;
    myCurrentIndexOffset += static_cast<uint>(inIndices.size());
    return buffer;
}

IndexBufferHandle* IndexBufferSystem::UploadIndexBuffer(VulkanBuffer* inStagingBuffer, const uint inIndexCount)
{
    ZoneScoped;
    if(inIndexCount == 0)
    {
        LOG_WARNING("IndexBufferSystem::UploadIndexBuffer - Empty indices list");
        return nullptr;
    }

    // Upload the new data to the index data buffer.
    uint dataIndex = -1;
    if(!myFreeSparseIndices.IsEmpty())
    {
        dataIndex = myFreeSparseIndices.Last();
        myFreeSparseIndices.RemoveLast();
    }
    else
    {
        dataIndex = mySparseIndexData_CPURepresentation.size();
        mySparseIndexData_CPURepresentation.Emplace();
    }
    
    IndexBufferData& data = mySparseIndexData_CPURepresentation[dataIndex];
    data.myOffset = myCurrentIndexOffset;
    data.myCount = inIndexCount;
    
    IndexBufferHandle* buffer = new IndexBufferHandle();
    const uint sizeIncrease = inIndexCount * sizeof(uint);

    myBuffer->CopyDataFromBuffer(inStagingBuffer, sizeIncrease, myUsedBufferSize);
    mySparseIndexDataBuffer->SetData(&data, sizeof(IndexBufferData), sizeof(IndexBufferData) * dataIndex);
    
    buffer->myIndex = dataIndex;
    myIndexBuffers.Add(buffer);
    
    myUsedBufferSize += sizeIncrease;
    myCurrentIndexOffset += static_cast<uint>(inIndexCount);
    return buffer;
}

const IndexBufferData& IndexBufferSystem::GetIndexBufferDataFromIndexHandle(IndexBufferHandle* inHandle)
{
    return mySparseIndexData_CPURepresentation[inHandle->myIndex];
}

void IndexBufferSystem::RemoveIndexBuffer(const IndexBufferHandle* inBuffer)
{
    LOG_WARNING("IndexBufferSubsystem::RemoveIndexBuffer not implemented.");
}