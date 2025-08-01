#include "EnginePch.h"
#include "PointLightSystem.h"

#include "Engine/Components/PointLightComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Shaders/MeshStructs.hpp"
#include "Engine/Vulkan/ResizableBuffer.h"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanSwapChain.h"

PointLightSystem::PointLightSystem()
{
    myBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("PointLightBuffer", VulkanBuffer::ResizableStorageBufferCreateInfo(sizeof(PointLightData) * 16), VMA_MEMORY_USAGE_AUTO, false));
}

PointLightSystem::~PointLightSystem()
{
    VulkanAllocator::DestroyBuffer_TS(myBuffer);    
}

const ResizableBuffer* PointLightSystem::GetBuffer() const
{
    return myBuffer;    
}

void PointLightSystem::AddLight(TransformComponent& inTransform, PointLightComponent* inLight)
{
    PointLightData pointLightData{};
    pointLightData.myColor = glm::vec4(inLight->GetColor(), 1.0f);
    pointLightData.myRange = inLight->GetRange();
    pointLightData.myIntensity = inLight->GetIntensity();
    pointLightData.myPosition = inTransform.GetPosition();
    
    constexpr int lengthByteOffset = 32;
    myBuffer->SetData(&pointLightData, sizeof(PointLightData), lengthByteOffset + sizeof(PointLightData) * myNumPointLights);
    
    myNumPointLights++;
    myBuffer->SetData(&myNumPointLights, sizeof(uint), 0);

    LOG("Adding pointlight on frame: %i", VulkanContext::GetSwapChain().GetFrameIndex());
}