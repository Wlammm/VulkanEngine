#include "EnginePch.h"
#include "IndirectPrePass.h"

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
    inCommandBuffer.fillBuffer(RenderSystem::Get()->myCountBuffer->GetAPIResource(), 0, sizeof(uint32_t), 0);
    inCommandBuffer.fillBuffer(RenderSystem::Get()->myCountNoDepthBuffer->GetAPIResource(), 0, sizeof(uint32_t), 0);
    
    {
        // Insert memory barrier to ensure that the count buffer is synchronized before cull pass
        vk::BufferMemoryBarrier barrier = vk::BufferMemoryBarrier()
            .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)  // Pre-pass writes
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite)   // Cull pass reads & writes
            .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setBuffer(RenderSystem::Get()->myCountBuffer->GetAPIResource())
            .setOffset(0)
            .setSize(VK_WHOLE_SIZE);

        inCommandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eComputeShader,
                vk::PipelineStageFlagBits::eComputeShader,
                vk::DependencyFlags{},
                nullptr,
                barrier,
                nullptr);
    }
    
    {
        // Insert memory barrier to ensure that the count buffer is synchronized before cull pass
        vk::BufferMemoryBarrier barrier = vk::BufferMemoryBarrier()
            .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)  // Pre-pass writes
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite)   // Cull pass reads & writes
            .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
            .setBuffer(RenderSystem::Get()->myCountNoDepthBuffer->GetAPIResource())
            .setOffset(0)
            .setSize(VK_WHOLE_SIZE);

        inCommandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eComputeShader,
                vk::PipelineStageFlagBits::eComputeShader,
                vk::DependencyFlags{},
                nullptr,
                barrier,
                nullptr);
    }
}
