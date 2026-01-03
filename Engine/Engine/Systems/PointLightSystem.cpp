#include "EnginePch.h"
#include "PointLightSystem.h"

#include "Engine/Shaders/Shared/MeshStructs.hpp"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanContext.h"

PointLightSystem::PointLightSystem() : myPointLightBuffer(VulkanBuffer::ResizableStorageBufferCreateInfo(
                                                                          sizeof(PointLightData) * 16), "PointLightBuffer", VMA_MEMORY_USAGE_AUTO, 16)
{ }

PointLightInstanceIndex PointLightSystem::AddLight(const PointLightData& inPointLightData)
{
    return myPointLightBuffer.Add(inPointLightData);
}

void PointLightSystem::UpdateLight(const PointLightInstanceIndex inLightInstance, const PointLightData& inPointLightData)
{
    myPointLightBuffer.Update(inPointLightData, inLightInstance);
}

void PointLightSystem::RemoveLight(const PointLightInstanceIndex inLightInstance)
{
    myPointLightBuffer.Remove(inLightInstance);
}

uint PointLightSystem::GetNumPointLights() const
{
    return myPointLightBuffer.Size();
}
