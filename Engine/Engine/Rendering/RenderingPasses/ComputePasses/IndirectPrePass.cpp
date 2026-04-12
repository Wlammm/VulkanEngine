#include "EnginePch.h"
#include "IndirectPrePass.h"

#include "Engine/Shaders/Shared/MeshStructs.hpp"

IndirectPrePass::IndirectPrePass()
{
}

void IndirectPrePass::CreateResources()
{
}

void IndirectPrePass::DestroyResources()
{
}

void IndirectPrePass::Execute(vk::CommandBuffer inCommandBuffer)
{
    GPUMARK_SCOPE(inCommandBuffer, "IndirectPrePass");
    inCommandBuffer.fillBuffer(RenderSystem::Get()->myCountBuffer->GetAPIResource(), 0, sizeof(uint32_t) * EShadingBin::ShadingBin_Count, 0);
    
    {
        // Insert memory barrier to ensure that the count buffer is synchronized before cull pass
        vk::BufferMemoryBarrier barrier = vk::BufferMemoryBarrier()
            .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)  // fillBuffer is a transfer operation
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite)   // Cull pass reads & writes
            .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setBuffer(RenderSystem::Get()->myCountBuffer->GetAPIResource())
            .setOffset(0)
            .setSize(VK_WHOLE_SIZE);

        inCommandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eComputeShader,
                vk::DependencyFlags{},
                nullptr,
                barrier,
                nullptr);
    }
}
