#include "EnginePch.h"
#include "PointLightSystem.h"

#include "Components/PointLightComponent.h"
#include "Components/TransformComponent.h"
#include "Shaders/MeshStructs.hpp"
#include "Vulkan/ResizableBuffer.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"

PointLightSystem::PointLightSystem()
{
    myBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("PointLightBuffer", VulkanBuffer::ResizableStorageBufferCreateInfo(32), VMA_MEMORY_USAGE_AUTO, false));
}

PointLightSystem::~PointLightSystem()
{
    VulkanAllocator::DestroyBuffer_TS(myBuffer);    
}

const ResizableBuffer* PointLightSystem::GetBuffer() const
{
    return myBuffer;    
}

void PointLightSystem::AddLight(TransformComponent* inTransform, PointLightComponent* inLight)
{
    PointLightData pointLightData{};
    pointLightData.myColor = glm::vec4(inLight->GetColor(), 1.0f);
    pointLightData.myRange = inLight->GetRange();
    pointLightData.myIntensity = inLight->GetIntensity();
    pointLightData.myPosition = inTransform->GetPosition();

    constexpr int lengthByteOffset = 32;
    myBuffer->SetData(&pointLightData, sizeof(PointLightData), lengthByteOffset + sizeof(PointLightData) * myNumPointLights);
    
    myNumPointLights++;
    myBuffer->SetData(&myNumPointLights, sizeof(uint), 0);
}