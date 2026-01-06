#include "EnginePch.h"
#include "NoDepthPass.h"

#include "Engine/Rendering/GPUResourceManager.h"
#include "Engine/Systems/PointLightSystem.h"
#include "Engine/Vulkan/GPUSceneSystem.h"
#include "Engine/Vulkan/ResizableBuffer.h"
#include "Engine/Vulkan/Containers/ConstantBuffer.h"

NoDepthPass::NoDepthPass()
: GraphicsPass("Shaders/MainVS.hlsl", "Shaders/MainPS.hlsl")
{
    
}

void NoDepthPass::SetupDescriptors()
{
    BindBuffer(
            GPUResourceManager::Get()->GetBuffer<CameraBuffer>(), 
            vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 
            0, 
            vk::DescriptorType::eUniformBuffer);
	
    BindBuffer(
        GPUResourceManager::Get()->GetBuffer<PointLightData>(), 
        vk::ShaderStageFlagBits::eFragment, 
        1, 
        vk::DescriptorType::eStorageBuffer);
	
    BindBuffer(
        GPUResourceManager::Get()->GetBuffer<DirectionalLightBuffer>(),
        vk::ShaderStageFlagBits::eFragment, 
        2, 
        vk::DescriptorType::eUniformBuffer);

    BindBuffer(
        GPUResourceManager::Get()->GetBuffer<PerDrawData>(),
        vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex, 
        4, 
        vk::DescriptorType::eStorageBuffer);
    
    BindBuffer(
       GPUResourceManager::Get()->GetBuffer<SceneHeader>(),
       vk::ShaderStageFlagBits::eFragment, 
       5, 
       vk::DescriptorType::eUniformBuffer);
	
    Build();
    
    SetPushConstantToType<ShadingBinHeader>(vk::ShaderStageFlagBits::eVertex);
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
