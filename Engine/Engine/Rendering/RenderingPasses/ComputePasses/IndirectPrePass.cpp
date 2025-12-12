#include "EnginePch.h"
#include "IndirectPrePass.h"

#include "Engine/Rendering/GDRPipeline.h"

IndirectPrePass::IndirectPrePass()
    : ComputePass("Shaders/PrePass.comp")
{
}

void IndirectPrePass::SetupDescriptors()
{
    myDescriptorSet.BindBuffer(GDRPipeline::Get()->myCountBuffer, vk::ShaderStageFlagBits::eCompute, 0, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(GDRPipeline::Get()->myCountNoDepthBuffer, vk::ShaderStageFlagBits::eCompute, 1, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.Build();
}

void IndirectPrePass::DispatchCall(vk::CommandBuffer inCommandBuffer)
{
    // TODO: Maybe this could just be a write buffer immediate instead?
    inCommandBuffer.dispatch(1, 1, 1);
    
    // Insert memory barrier to ensure that the count buffer is synchronized before cull pass
    vk::BufferMemoryBarrier barrier = vk::BufferMemoryBarrier()
        .setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)  // Pre-pass writes
        .setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite)   // Cull pass reads & writes
        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setBuffer(GDRPipeline::Get()->myCountBuffer->GetAPIResource())
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
