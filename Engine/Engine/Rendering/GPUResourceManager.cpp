#include "EnginePch.h"
#include "GPUResourceManager.h"

#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"

GPUResourceManager::GPUResourceManager()
{
    check(!myInstance);
    myInstance = this;
}

GPUResourceManager::~GPUResourceManager()
{
    myInstance = nullptr;
    
    for (BufferResource& buffer : myBuffers)
    {
        if (!buffer.myIsOwned)
            continue;

        // TODO: Fix this hack later on.
        if (VulkanBuffer* vulkanBuffer = dynamic_cast<VulkanBuffer*>(buffer.myBuffer))
        {
            VulkanAllocator::DestroyBuffer_TS(vulkanBuffer);
        }
        else
        {
            del(buffer.myBuffer);
        }
    }
    
    myBuffers.Clear();
}

void GPUResourceManager::Tick()
{
    for (BufferResource& buffer : myTickableBuffers)
    {
        buffer.myTickFunction(buffer);
    }
}

IGPUBuffer* GPUResourceManager::GetBuffer(const std::string& inBufferTypeName) const
{
    const Type* type = ReflectionSystem::GetTypeByName(inBufferTypeName);
    return GetBuffer(type);
}

IGPUBuffer* GPUResourceManager::TryGetBuffer(const std::string& inBufferTypeName) const
{
    const Type* type = ReflectionSystem::GetTypeByName(inBufferTypeName);
    if (!type)
        return nullptr;

    for (const BufferResource& buffer : myBuffers)
    {
        if (buffer.myType == type)
            return buffer.myBuffer;
    }
    return nullptr;
}

IGPUBuffer* GPUResourceManager::TryGetBufferByAlias(const std::string& inAlias) const
{
    if (inAlias.empty())
        return nullptr;

    for (const BufferResource& buffer : myBuffers)
    {
        for (const std::string& alias : buffer.myShaderAliases)
        {
            if (alias == inAlias)
                return buffer.myBuffer;
        }
    }
    return nullptr;
}

void GPUResourceManager::RegisterBuffer(IGPUBuffer* inBuffer, List<std::string> inShaderAliases)
{
    BufferResource& buffer = myBuffers.Emplace();
    buffer.myBuffer = inBuffer;
    buffer.myShaderAliases = std::move(inShaderAliases);
    buffer.myIsOwned = false;
}

IGPUBuffer* GPUResourceManager::GetBuffer(const Type* inType) const
{
    for (const BufferResource& buffer : myBuffers)
    {
        if (buffer.myType == inType)
            return buffer.myBuffer;
    }
        
    check(false && "Failed to find buffer for this type.");
    return nullptr;
}
