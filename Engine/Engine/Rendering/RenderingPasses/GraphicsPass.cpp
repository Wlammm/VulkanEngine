#include "EnginePch.h"
#include "GraphicsPass.h"

#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Assets/Shader.h"
#include "Engine/Rendering/IndexBufferSystem.h"
#include "Engine/Rendering/TextureSystem.h"
#include "Engine/Rendering/VertexBufferSystem.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanPhysicalDevice.h"

GraphicsPass::GraphicsPass(const SourcePath& inVertexShaderPath, const SourcePath& inFragmentShaderPath)
{
    myVertexShader = AssetRegistry::Get()->GetAssetSynchronous<Shader>(inVertexShaderPath);
    myVertexShader->OnShaderRecompiled.Bind(&GraphicsPass::OnShaderRecompiled, this);
    
    myFragmentShader = AssetRegistry::Get()->GetAssetSynchronous<Shader>(inFragmentShaderPath);
    myFragmentShader->OnShaderRecompiled.Bind(&GraphicsPass::OnShaderRecompiled, this);
}

GraphicsPass::~GraphicsPass()
{
    myVertexShader->OnShaderRecompiled.UnBind(&GraphicsPass::OnShaderRecompiled, this);
    myFragmentShader->OnShaderRecompiled.UnBind(&GraphicsPass::OnShaderRecompiled, this);
}

void GraphicsPass::Execute(vk::CommandBuffer inCommandBuffer)
{
    VertexBufferSystem& vertexBufferSystem = Engine::GetEngineSystem<VertexBufferSystem>();
    IndexBufferSystem& indexBufferSystem = Engine::GetEngineSystem<IndexBufferSystem>();
    
    inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);
    inCommandBuffer.bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics,
        myPipelineLayout, 
        0,
        myDescriptorSet.GetSet(), 
        {});

    TextureSystem& textureSystem = Engine::GetEngineSystem<TextureSystem>();
    inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 1, textureSystem.GetDescriptorSet(), {});

    inCommandBuffer.bindVertexBuffers(0, {vertexBufferSystem.GetGlobalVertexBuffer()->GetBuffer()->GetAPIResource()}, {0});
	inCommandBuffer.bindIndexBuffer(indexBufferSystem.GetGlobalIndexBuffer()->GetBuffer()->GetAPIResource(),0, vk::IndexType::eUint32);

    
    DrawCall(inCommandBuffer);
}

void GraphicsPass::CreateResources()
{
    myDescriptorSet = {};
    SetupDescriptors();
 
    // TODO: We should probably create some uniform descriptor set layouts both in shaders and in engine to make it easier to keep track of what is needed where.
    TextureSystem& textureSystem = Engine::GetEngineSystem<TextureSystem>();
    
    const List<vk::DescriptorSetLayout> layouts{ myDescriptorSet.GetLayout(), textureSystem.GetDescriptorLayout() };
    myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));
    
    const std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageInfo = {
        vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eVertex).setModule(myVertexShader->GetAPIResource()).setPName("main"),
        vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eFragment).setModule(myFragmentShader->GetAPIResource()).setPName("main"),
    };
    
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo = vk::PipelineVertexInputStateCreateInfo().setVertexAttributeDescriptions(Vertex::GetAttributeDescriptions()).setVertexBindingDescriptions(Vertex::GetBindingDescriptions());

    const vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo().setTopology(vk::PrimitiveTopology::eTriangleList);
    const vk::PipelineViewportStateCreateInfo viewportInfo = vk::PipelineViewportStateCreateInfo().setViewportCount(1).setScissorCount(1);
    const vk::PipelineRasterizationStateCreateInfo rasterizationInfo = vk::PipelineRasterizationStateCreateInfo()
        .setDepthClampEnable(VK_FALSE)
        .setRasterizerDiscardEnable(VK_FALSE)
        .setPolygonMode(vk::PolygonMode::eFill)
        .setCullMode(vk::CullModeFlagBits::eFront)
        .setFrontFace(vk::FrontFace::eCounterClockwise)
        .setDepthBiasEnable(VK_FALSE)
        .setLineWidth(1.0f);
    
    const auto multisampleInfo = vk::PipelineMultisampleStateCreateInfo().setRasterizationSamples(VulkanContext::GetPhysicalDevice().GetMaxMSAASamples());
    const auto stencilOp = vk::StencilOpState().setFailOp(vk::StencilOp::eKeep).setPassOp(vk::StencilOp::eKeep).setCompareOp(vk::CompareOp::eAlways);

    const auto depthStencilInfo = vk::PipelineDepthStencilStateCreateInfo()
        .setDepthTestEnable(VK_TRUE)
        .setDepthWriteEnable(VK_TRUE)
        .setDepthCompareOp(vk::CompareOp::eLessOrEqual)
        .setDepthBoundsTestEnable(VK_FALSE)
        .setStencilTestEnable(VK_FALSE)
        .setFront(stencilOp)
        .setBack(stencilOp);

    const std::array<vk::PipelineColorBlendAttachmentState, 1> colorBlendAttachments = {
        vk::PipelineColorBlendAttachmentState().setColorWriteMask(
            vk::ColorComponentFlagBits::eR |
            vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB |
            vk::ColorComponentFlagBits::eA) };

    const vk::PipelineColorBlendStateCreateInfo colorBlendInfo = vk::PipelineColorBlendStateCreateInfo().setAttachments(colorBlendAttachments);
    const std::array<vk::DynamicState, 3> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor, vk::DynamicState::eDepthWriteEnable };
    const vk::PipelineDynamicStateCreateInfo dynamicStateInfo = vk::PipelineDynamicStateCreateInfo().setDynamicStates(dynamicStates);

    const vk::ResultValue<vk::Pipeline> returnValue = VulkanContext::GetDevice()->createGraphicsPipeline(VulkanContext::GetPipelineCache(), vk::GraphicsPipelineCreateInfo()
                                                       .setStages(shaderStageInfo)
                                                       .setPVertexInputState(&vertexInputInfo)
                                                       .setPInputAssemblyState(&inputAssemblyInfo)
                                                       .setPViewportState(&viewportInfo)
                                                       .setPRasterizationState(&rasterizationInfo)
                                                       .setPMultisampleState(&multisampleInfo)
                                                       .setPDepthStencilState(&depthStencilInfo)
                                                       .setPColorBlendState(&colorBlendInfo)
                                                       .setPDynamicState(&dynamicStateInfo)
                                                       .setLayout(myPipelineLayout)
                                                       .setRenderPass(Engine::GetEngineSystem<RenderSystem>().GetRenderPass()));

    check(returnValue.result == vk::Result::eSuccess);
    myPipeline = returnValue.value;
}

void GraphicsPass::DestroyResources()
{
    VulkanContext::GetDevice()->destroyPipeline(myPipeline);
    VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
}

void GraphicsPass::OnShaderRecompiled()
{
    DestroyResources();
    CreateResources();
}
