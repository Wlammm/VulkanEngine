#include "EnginePch.h"
#include "FoliagePrePass.h"

#include "Engine/Engine.h"
#include "Engine/Rendering/FoliageSystem.h"
#include "Engine/Vulkan/VulkanBuffer.h"

void FoliagePrePass::PreExecute()
{
    IRenderPass::PreExecute();

    VulkanBuffer* countBuffer = Engine::GetEngineSystem<FoliageSystem>().GetCountBuffer();
    RegisterDynamicBufferUsage(countBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferWrite);
}

void FoliagePrePass::Execute(vk::CommandBuffer inCommandBuffer)
{
    GPUMARK_SCOPE(inCommandBuffer, "FoliagePrePass");

    VulkanBuffer* countBuffer = Engine::GetEngineSystem<FoliageSystem>().GetCountBuffer();
    inCommandBuffer.fillBuffer(countBuffer->GetAPIResource(), 0, sizeof(uint32_t), 0);
}
