#include "EnginePch.h"
#include "MainPass.h"

#include "Engine/Systems/PointLightSystem.h"
#include "Engine/Vulkan/ResizableBuffer.h"

MainPass::MainPass()
    : GraphicsPass("Shaders/IndirectVertexShader.vert", "Shaders/IndirectFragmentShader.frag")
{
}

void MainPass::SetupDescriptors()
{
    myDescriptorSet.BindBuffer(
            RenderSystem::Get()->myFrameDataBuffer, 
            vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 
            0, 
            vk::DescriptorType::eUniformBuffer);
	
    myDescriptorSet.BindBuffer(
        Engine::GetEngineSystem<PointLightSystem>().GetBuffer(), 
        vk::ShaderStageFlagBits::eFragment, 
        1, 
        vk::DescriptorType::eStorageBuffer);
	
    myDescriptorSet.BindBuffer(
        RenderSystem::Get()->myDirectionalLightBuffer,
        vk::ShaderStageFlagBits::eFragment, 
        2, 
        vk::DescriptorType::eUniformBuffer);

    myDescriptorSet.BindBuffer(
        RenderSystem::Get()->myPerDrawDataBuffer,
        vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex, 
        4, 
        vk::DescriptorType::eStorageBuffer);
	
    myDescriptorSet.Build();
}

void MainPass::SetupAttachments()
{
    AddColorAttachment(RenderSystem::Get()->myRenderTexture, vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedRenderTexture());
    
    AddDepthAttachment(RenderSystem::Get()->myDepthBuffer, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore);
}

void MainPass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.setDepthWriteEnable(true);
    
    inCommandBuffer.drawIndexedIndirectCount(RenderSystem::Get()->myIndirectCommandsBuffer->GetBuffer()->GetAPIResource(), 0,
    RenderSystem::Get()->myCountBuffer->GetAPIResource(), 0,
    static_cast<uint>(RenderSystem::Get()->myIndirectCommandsBuffer->GetBuffer()->GetSize() / sizeof(vk::DrawIndexedIndirectCommand)),
    sizeof(vk::DrawIndexedIndirectCommand));
}
