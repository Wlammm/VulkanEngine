#include "EnginePch.h"
#include "IndirectPrePass.h"

#include "Engine/Shaders/Shared/MeshStructs.hpp"

void IndirectPrePass::PreExecute()
{
    IRenderPass::PreExecute();
    RegisterDynamicBufferUsage(RenderSystem::Get()->myCountBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferWrite);
}

void IndirectPrePass::Execute(vk::CommandBuffer inCommandBuffer)
{
    GPUMARK_SCOPE(inCommandBuffer, "IndirectPrePass");
    inCommandBuffer.fillBuffer(RenderSystem::Get()->myCountBuffer->GetAPIResource(), 0, sizeof(uint32_t) * EShadingBin::ShadingBin_Count, 0);
}
