#include "EnginePch.h"
#include "IndirectPrePass.h"

#include "Engine/Shaders/Shared/MeshStructs.hpp"

IndirectPrePass::IndirectPrePass()
{
}

void IndirectPrePass::CreateResources()
{
    RegisterBufferUsage(RenderSystem::Get()->myCountBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferWrite);
}

void IndirectPrePass::DestroyResources()
{
}

void IndirectPrePass::Execute(vk::CommandBuffer inCommandBuffer)
{
    GPUMARK_SCOPE(inCommandBuffer, "IndirectPrePass");
    inCommandBuffer.fillBuffer(RenderSystem::Get()->myCountBuffer->GetAPIResource(), 0, sizeof(uint32_t) * EShadingBin::ShadingBin_Count, 0);
}
