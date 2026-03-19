#include "EnginePch.h"
#include "NoDepthPass.h"

#include "Engine/Rendering/GPUResourceManager.h"
#include "Engine/Vulkan/GPUSceneSystem.h"

NoDepthPass::NoDepthPass()
: GraphicsPass("Shaders/MainVS.hlsl", "Shaders/MainPS.hlsl")
{
    
}

void NoDepthPass::SetupAttachments()
{
    AddColorAttachment(RenderSystem::Get()->myRenderTexture, vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore);
    
    AddDepthAttachment(RenderSystem::Get()->myDepthBuffer, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::AttachmentLoadOp::eLoad, vk::AttachmentStoreOp::eStore);
}

void NoDepthPass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.setDepthWriteEnable(false);

    uint maxNumDraws = Engine::GetEngineSystem<GPUSceneSystem>().GetNumObjects();
    
    ShadingBinHeader header;
    header.myShadingBin = EShadingBin::ShadingBin_NoDepth;
    header.myElementsPerBin = maxNumDraws;
    inCommandBuffer.pushConstants(myPipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(ShadingBinHeader), &header);
    
    inCommandBuffer.drawIndexedIndirectCount(
        GPUResourceManager::Get()->GetBuffer<vk::DrawIndexedIndirectCommand>()->GetBuffer()->GetAPIResource(), 
        maxNumDraws * EShadingBin::ShadingBin_NoDepth * sizeof(vk::DrawIndexedIndirectCommand),
        RenderSystem::Get()->myCountBuffer->GetAPIResource(), 
        EShadingBin::ShadingBin_NoDepth * sizeof(uint),
        maxNumDraws,
        sizeof(vk::DrawIndexedIndirectCommand));
}
