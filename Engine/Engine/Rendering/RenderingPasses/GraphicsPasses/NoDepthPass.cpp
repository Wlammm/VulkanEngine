#include "EnginePch.h"
#include "NoDepthPass.h"

#include "Engine/Rendering/GDRPipeline.h"
#include "Engine/Systems/PointLightSystem.h"
#include "Engine/Vulkan/ResizableBuffer.h"

NoDepthPass::NoDepthPass()
: GraphicsPass("Shaders/IndirectVertexShader.vert", "Shaders/IndirectFragmentShader.frag")
{
    
}

void NoDepthPass::SetupDescriptors()
{
    myDescriptorSet.BindBuffer(
            GDRPipeline::Get()->myFrameDataBuffer, 
            vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 
            0, 
            vk::DescriptorType::eUniformBuffer);
	
    myDescriptorSet.BindBuffer(
        Engine::GetEngineSystem<PointLightSystem>().GetBuffer(), 
        vk::ShaderStageFlagBits::eFragment, 
        1, 
        vk::DescriptorType::eStorageBuffer);
	
    myDescriptorSet.BindBuffer(
        GDRPipeline::Get()->myDirectionalLightBuffer,
        vk::ShaderStageFlagBits::eFragment, 
        2, 
        vk::DescriptorType::eUniformBuffer);

    myDescriptorSet.BindBuffer(
        GDRPipeline::Get()->myPerDrawDataNoDepthBuffer,
        vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex, 
        4, 
        vk::DescriptorType::eStorageBuffer);
	
    myDescriptorSet.Build();
}

void NoDepthPass::DrawCall(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.setDepthWriteEnable(false);

    inCommandBuffer.drawIndexedIndirectCount(GDRPipeline::Get()->myIndirectCommandsBufferNoDepth->GetBuffer()->GetAPIResource(), 0,
            GDRPipeline::Get()->myCountNoDepthBuffer->GetAPIResource(), 0,
            static_cast<uint>(GDRPipeline::Get()->myIndirectCommandsBufferNoDepth->GetBuffer()->GetSize() / sizeof(vk::DrawIndexedIndirectCommand)),
            sizeof(vk::DrawIndexedIndirectCommand));
}
