#include "EnginePch.h"
#include "MainPass.h"

#include "Engine/Rendering/GPUResourceManager.h"
#include "Engine/Vulkan/GPUSceneSystem.h"

MainPass::MainPass()
    : GraphicsPass("Shaders/MainVS.hlsl", "Shaders/MainPS.hlsl")
{
}

void MainPass::SetupDescriptors()
{
    SetPushConstantToType<ShadingBinHeader>(vk::ShaderStageFlagBits::eVertex);
}

void MainPass::SetupAttachments()
{
    AddColorAttachment(RenderSystem::Get()->myRenderTexture, vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedRenderTexture());
    
    AddDepthAttachment(RenderSystem::Get()->myDepthBuffer, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, RenderSystem::Get()->GetResolvedDepthTexture());
}

void MainPass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.setDepthWriteEnable(true);
    
    uint maxNumDraws = Engine::GetEngineSystem<GPUSceneSystem>().GetNumObjects();
    
    ShadingBinHeader header;
    header.myShadingBin = EShadingBin::ShadingBin_Default;
    header.myElementsPerBin = maxNumDraws;
    inCommandBuffer.pushConstants(myPipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(ShadingBinHeader), &header);
    
    inCommandBuffer.drawIndexedIndirectCount(
        GPUResourceManager::Get()->GetBuffer<vk::DrawIndexedIndirectCommand>()->GetBuffer()->GetAPIResource(), 
        maxNumDraws * EShadingBin::ShadingBin_Default * sizeof(vk::DrawIndexedIndirectCommand),
        RenderSystem::Get()->myCountBuffer->GetAPIResource(), 
        EShadingBin::ShadingBin_Default * 4,
        maxNumDraws,
        sizeof(vk::DrawIndexedIndirectCommand));
}
