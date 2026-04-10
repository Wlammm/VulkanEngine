#include "EnginePch.h"
#include "GPUResourceManager.h"

#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"

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

    for (AccelerationStructureResource& asResource : myAccelerationStructures)
    {
        if (!asResource.myIsOwned)
            continue;

        if (asResource.myAccelerationStructure)
            VulkanContext::GetDevice().GetDevice().destroyAccelerationStructureKHR(asResource.myAccelerationStructure);

        if (asResource.myBuffer)
            VulkanAllocator::DestroyBuffer_TS(asResource.myBuffer);
    }
    myAccelerationStructures.Clear();
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
    // If an existing non-owned entry shares any alias, update it in-place so that
    // lookups immediately return the new buffer (e.g. after a resize recreates it).
    for (BufferResource& existing : myBuffers)
    {
        if (existing.myIsOwned)
            continue;
        for (const std::string& alias : inShaderAliases)
        {
            if (existing.myShaderAliases.Contains(alias))
            {
                existing.myBuffer = inBuffer;
                existing.myShaderAliases = std::move(inShaderAliases);
                return;
            }
        }
    }

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

void GPUResourceManager::RegisterAccelerationStructure(vk::AccelerationStructureKHR inAS, VulkanBuffer* inBuffer,
                                                        List<std::string> inShaderAliases, bool inIsOwned)
{
    // Update in-place if any alias already exists (e.g. TLAS rebuilt each frame).
    for (AccelerationStructureResource& existing : myAccelerationStructures)
    {
        for (const std::string& alias : inShaderAliases)
        {
            if (existing.myShaderAliases.Contains(alias))
            {
                existing.myAccelerationStructure = inAS;
                existing.myBuffer = inBuffer;
                existing.myShaderAliases = std::move(inShaderAliases);
                existing.myIsOwned = inIsOwned;
                return;
            }
        }
    }

    AccelerationStructureResource& resource = myAccelerationStructures.Emplace();
    resource.myAccelerationStructure = inAS;
    resource.myBuffer = inBuffer;
    resource.myShaderAliases = std::move(inShaderAliases);
    resource.myIsOwned = inIsOwned;
}

vk::AccelerationStructureKHR GPUResourceManager::GetAccelerationStructure(const std::string& inAlias) const
{
    vk::AccelerationStructureKHR result = TryGetAccelerationStructure(inAlias);
    check(result && "Failed to find acceleration structure for this alias.");
    return result;
}

vk::AccelerationStructureKHR GPUResourceManager::TryGetAccelerationStructure(const std::string& inAlias) const
{
    for (const AccelerationStructureResource& resource : myAccelerationStructures)
    {
        for (const std::string& alias : resource.myShaderAliases)
        {
            if (alias == inAlias)
                return resource.myAccelerationStructure;
        }
    }
    return {};
}
