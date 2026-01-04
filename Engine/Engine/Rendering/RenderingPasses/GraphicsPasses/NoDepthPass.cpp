#include "EnginePch.h"
#include "NoDepthPass.h"

#include "Engine/Rendering/GPUResourceManager.h"
#include "Engine/Systems/PointLightSystem.h"
#include "Engine/Vulkan/ResizableBuffer.h"

NoDepthPass::NoDepthPass()
: GraphicsPass("Shaders/MainVS.hlsl", "Shaders/MainPS.hlsl")
{
    
}

void NoDepthPass::SetupDescriptors()
{
    myDescriptorSet.BindBuffer(
            GPUResourceManager::Get()->GetBuffer<CameraBuffer>(), 
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
        RenderSystem::Get()->myPerDrawDataNoDepthBuffer,
        vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex, 
        4, 
        vk::DescriptorType::eStorageBuffer);
    
    myDescriptorSet.BindBuffer(
       GPUResourceManager::Get()->GetBuffer<SceneHeader>(),
       vk::ShaderStageFlagBits::eFragment, 
       5, 
       vk::DescriptorType::eUniformBuffer);
	
    myDescriptorSet.Build();
}

void NoDepthPass::SetupAttachments()
{
    AddColorAttachment(RenderSystem::Get()->myRenderTexture, vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore);
    
    AddDepthAttachment(RenderSystem::Get()->myDepthBuffer, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore);
}

void NoDepthPass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.setDepthWriteEnable(false);

    inCommandBuffer.drawIndexedIndirectCount(RenderSystem::Get()->myIndirectCommandsBufferNoDepth->GetBuffer()->GetAPIResource(), 0,
            RenderSystem::Get()->myCountNoDepthBuffer->GetAPIResource(), 0,
            static_cast<uint>(RenderSystem::Get()->myIndirectCommandsBufferNoDepth->GetBuffer()->GetSize() / sizeof(vk::DrawIndexedIndirectCommand)),
            sizeof(vk::DrawIndexedIndirectCommand));
}
