#include "EnginePch.h"
#include "GraphicsPass.h"

#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Assets/Shader.h"
#include "Engine/Rendering/GPUResourceManager.h"
#include "Engine/Rendering/IndexBufferSystem.h"
#include "Engine/Rendering/TextureSystem.h"
#include "Engine/Rendering/VertexBufferSystem.h"
#include "Engine/Rendering/RenderGraph/RenderGraph.h"
#include "Engine/Vulkan/GPUSceneSystem.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanPhysicalDevice.h"
#include "Engine/Vulkan/VulkanSwapChain.h"

GraphicsPass::GraphicsPass(const SourcePath& inVertexShaderPath, const SourcePath& inFragmentShaderPath, const bool inDynamicAttachments, const bool inNeedsBindlessTexturesDescriptor)
{
    myHasDynamicAttachments = inDynamicAttachments;
    myNeedsBindlessTextureDescriptor = inNeedsBindlessTexturesDescriptor;
    
    myVertexShader = AssetRegistry::Get()->GetAssetSynchronous<Shader>(inVertexShaderPath);
    myVertexShader->OnShaderRecompiled.Bind(&GraphicsPass::OnShaderRecompiled, this);
    
    if (!inFragmentShaderPath.empty())
    {
        myFragmentShader = AssetRegistry::Get()->GetAssetSynchronous<Shader>(inFragmentShaderPath);
        myFragmentShader->OnShaderRecompiled.Bind(&GraphicsPass::OnShaderRecompiled, this);
    }
}

GraphicsPass::~GraphicsPass()
{
    myVertexShader->OnShaderRecompiled.UnBind(&GraphicsPass::OnShaderRecompiled, this);
    
    if (myFragmentShader)
        myFragmentShader->OnShaderRecompiled.UnBind(&GraphicsPass::OnShaderRecompiled, this);
}

vk::SampleCountFlagBits GraphicsPass::GetNumSamples() const
{
    return VulkanContext::GetPhysicalDevice().GetMaxMSAASamples();
}

List<vk::VertexInputBindingDescription>& GraphicsPass::GetVertexBindingDescriptor() const
{
    return Vertex::GetBindingDescriptions();
}

List<vk::VertexInputAttributeDescription>& GraphicsPass::GetVertexAttributeDescriptions() const
{
    return Vertex::GetAttributeDescriptions();
}

vk::PrimitiveTopology GraphicsPass::GetPrimitiveTopology() const
{
    return vk::PrimitiveTopology::eTriangleList;
}

void GraphicsPass::Execute(vk::CommandBuffer inCommandBuffer)
{
    GPUMARK_SCOPE(inCommandBuffer, myPassName.c_str());
    
    List<vk::RenderingAttachmentInfo> colorAttachment = myColorAttachments;
    vk::RenderingAttachmentInfo depthAttachment = myDepthAttachment;

    if (myHasDynamicAttachments)
    {
        check(!myDynamicColorAttachments.IsEmpty() && "Dynamic color attachments shouldnt be zero if we have dynamic color attachments.");
        colorAttachment = myDynamicColorAttachments;
        depthAttachment = myDynamicDepthAttachment;
    }
    
    VulkanBuffer* vertexBuffer = GPUResourceManager::Get()->GetBuffer<Vertex>()->GetBuffer();
    VulkanBuffer* indexBuffer = GPUResourceManager::Get()->GetBuffer<Index>()->GetBuffer();
    
    // Insert barriers here because you're not allowed to do it by default inside a dynamic render pass in vulkan.
    List<ResourceUsage> resourceUsages{};
    resourceUsages.Emplace().SetToBuffer(vertexBuffer, vk::PipelineStageFlagBits::eVertexInput, vk::AccessFlagBits::eVertexAttributeRead);
    resourceUsages.Emplace().SetToBuffer(indexBuffer, vk::PipelineStageFlagBits::eVertexInput, vk::AccessFlagBits::eIndexRead);
    GetRenderGraph()->InsertResourceBarriers(inCommandBuffer, resourceUsages);
    GetRenderGraph()->InsertResourceBarriers(inCommandBuffer, myAttachmentResourceUsages);
    
    vk::RenderingInfo renderingInfo = vk::RenderingInfo()
        .setColorAttachments(colorAttachment)
        .setPDepthAttachment(myHasDepthAttachment ? &depthAttachment : nullptr)
        .setLayerCount(1)
        .setRenderArea(vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())));
    inCommandBuffer.beginRendering(renderingInfo);
    
    inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, myPipeline);
    inCommandBuffer.bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics,
        myPipelineLayout, 
        0,
        myDescriptorSet.GetSet(), 
        {});
    
    // Default to true for now..
    inCommandBuffer.setDepthWriteEnable(true);

    inCommandBuffer.setViewport(0, vk::Viewport()
        .setX(0)
        .setY(0)
        .setWidth(static_cast<float>(Engine::GetRenderResolution().x))
        .setHeight(static_cast<float>(Engine::GetRenderResolution().y))
        .setMinDepth(0.0f)
        .setMaxDepth(1.0f));
    inCommandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D{}, vk::Extent2D(VulkanContext::GetSwapChain().GetWidth(), VulkanContext::GetSwapChain().GetHeight())));
    inCommandBuffer.setFrontFace(vk::FrontFace::eCounterClockwise);
    
    TextureSystem& textureSystem = Engine::GetEngineSystem<TextureSystem>();
    
    if (myNeedsBindlessTextureDescriptor)
        inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, myPipelineLayout, 1, textureSystem.GetDescriptorSet(), {});
    
    inCommandBuffer.bindVertexBuffers(0, {vertexBuffer->GetAPIResource()}, {0});
	inCommandBuffer.bindIndexBuffer(indexBuffer->GetAPIResource(),0, vk::IndexType::eUint32);
    
    DrawCall(inCommandBuffer);
    
    inCommandBuffer.endRendering();
}

void GraphicsPass::PreExecute()
{
    IRenderPass::PreExecute();
    if (myHasDynamicAttachments)
    {
        myDynamicColorAttachments.Clear();
        GetDynamicColorAttachments();
        GetDynamicDepthAttachments();
    }
}

void GraphicsPass::CreateResources()
{
    myDescriptorSet = {};
    myColorAttachments.Clear();
    myColorFormats.Clear();
    
#if !SHIPPING
    if (myPassName.empty())
        myPassName = ReflectionSystem::GetType(this)->GetName();
#endif
    SetupAttachments();
    BuildDescriptors({myVertexShader, myFragmentShader});
 
    // TODO: We should probably create some uniform descriptor set layouts both in shaders and in engine to make it easier to keep track of what is needed where.
    TextureSystem& textureSystem = Engine::GetEngineSystem<TextureSystem>();
    
    List<vk::DescriptorSetLayout> layouts{ myDescriptorSet.GetLayout() };
    
    if (myNeedsBindlessTextureDescriptor)
        layouts.Add(textureSystem.GetDescriptorLayout());
    
    vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo().setSetLayouts(layouts);
    
    if (myPushConstantRange.size != 0)
        pipelineLayoutCreateInfo.setPushConstantRanges(myPushConstantRange);
    
    myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(pipelineLayoutCreateInfo);
    
    List<vk::PipelineShaderStageCreateInfo> shaderStageInfo;
    shaderStageInfo.Emplace().setStage(vk::ShaderStageFlagBits::eVertex).setModule(myVertexShader->GetAPIResource()).setPName(myVertexShader->GetEntryPoint().c_str());
    
    if (myFragmentShader)
        shaderStageInfo.Emplace().setStage(vk::ShaderStageFlagBits::eFragment).setModule(myFragmentShader->GetAPIResource()).setPName(myFragmentShader->GetEntryPoint().c_str());
    
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo = vk::PipelineVertexInputStateCreateInfo().setVertexAttributeDescriptions(GetVertexAttributeDescriptions()).setVertexBindingDescriptions(GetVertexBindingDescriptor());

    const vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo().setTopology(GetPrimitiveTopology());
    const vk::PipelineViewportStateCreateInfo viewportInfo = vk::PipelineViewportStateCreateInfo().setViewportCount(1).setScissorCount(1);
    const vk::PipelineRasterizationStateCreateInfo rasterizationInfo = vk::PipelineRasterizationStateCreateInfo()
        .setDepthClampEnable(VK_FALSE)
        .setRasterizerDiscardEnable(VK_FALSE)
        .setPolygonMode(vk::PolygonMode::eFill)
        .setCullMode(vk::CullModeFlagBits::eFront)
        .setFrontFace(vk::FrontFace::eCounterClockwise)
        .setDepthBiasEnable(VK_FALSE)
        .setLineWidth(1.0f);
    
    const auto multisampleInfo = vk::PipelineMultisampleStateCreateInfo().setRasterizationSamples(GetNumSamples());
    const auto stencilOp = vk::StencilOpState().setFailOp(vk::StencilOp::eKeep).setPassOp(vk::StencilOp::eKeep).setCompareOp(vk::CompareOp::eAlways);

    // TODO: Allow reading but not writing depth later...
    const auto depthStencilInfo = vk::PipelineDepthStencilStateCreateInfo()
        .setDepthTestEnable(VK_TRUE)
        .setDepthCompareOp(vk::CompareOp::eLessOrEqual)
        .setDepthBoundsTestEnable(VK_FALSE)
        .setStencilTestEnable(VK_FALSE)
        .setFront(stencilOp)
        .setBack(stencilOp);

    List<vk::PipelineColorBlendAttachmentState> colorBlendAttachmentStates{};
    for (const vk::RenderingAttachmentInfo& colorAttachment : myColorAttachments)
    {
        colorBlendAttachmentStates.Add(vk::PipelineColorBlendAttachmentState().setColorWriteMask(
            vk::ColorComponentFlagBits::eR |
            vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB |
            vk::ColorComponentFlagBits::eA));
    }
    
    const vk::PipelineColorBlendStateCreateInfo colorBlendInfo = vk::PipelineColorBlendStateCreateInfo().setAttachments(colorBlendAttachmentStates);
    const std::array<vk::DynamicState, 4> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor, vk::DynamicState::eDepthWriteEnable, vk::DynamicState::eFrontFace };
    const vk::PipelineDynamicStateCreateInfo dynamicStateInfo = vk::PipelineDynamicStateCreateInfo().setDynamicStates(dynamicStates);

    vk::PipelineRenderingCreateInfo renderingInfo;
    renderingInfo.setColorAttachmentFormats(myColorFormats);
    
    if (myHasDepthAttachment)
        renderingInfo.setDepthAttachmentFormat(myDepthFormat);
    
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
                                                       .setPNext(&renderingInfo));

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

void GraphicsPass::AddColorAttachment(
    VulkanImage* inImage, 
    vk::ImageLayout inLayout, 
    vk::AttachmentLoadOp inLoadOp,
    vk::AttachmentStoreOp inStoreOp, 
    VulkanImage* myResolveImage)
{
    myColorAttachments.Emplace()
        .setLoadOp(inLoadOp)
        .setStoreOp(inStoreOp)
        .setImageLayout(inLayout)
        .setImageView(inImage->GetImageView())
        .setClearValue(vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} })));
        
    RegisterAttachmentUsage(inImage, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::AccessFlagBits::eColorAttachmentWrite, inLayout);
        
    if (myResolveImage)
    {
        myColorAttachments.Last()
            .setResolveImageLayout(vk::ImageLayout::eColorAttachmentOptimal).
            setResolveImageView(myResolveImage->GetImageView())
            .setResolveMode(vk::ResolveModeFlagBits::eAverage);
            
        RegisterAttachmentUsage(myResolveImage, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::AccessFlagBits::eColorAttachmentWrite, vk::ImageLayout::eColorAttachmentOptimal);
    }
        
    myColorFormats.Add(inImage->GetFormat());
}

void GraphicsPass::AddDynamicColorAttachment(
    VulkanImage* inImage,
    vk::ImageLayout inLayout,
    vk::AttachmentLoadOp inLoadOp,
    vk::AttachmentStoreOp inStoreOp)
{
    myDynamicColorAttachments.Add(vk::RenderingAttachmentInfo().setLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setImageLayout(inLayout)
        .setLoadOp(inLoadOp)
        .setStoreOp(inStoreOp)
        .setImageView(inImage->GetImageView())
        .setClearValue(vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} }))));

    RegisterDynamicImageUsage(inImage, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::AccessFlagBits::eColorAttachmentWrite, inLayout);
}

void GraphicsPass::RegisterDynamicColorAttachment(
    vk::Format inFormat, 
    vk::ImageLayout inLayout,
    vk::AttachmentLoadOp inLoadOp, 
    vk::AttachmentStoreOp inStoreOp, 
    VulkanImage* myResolveImage)
{
    myColorAttachments.Emplace()
            .setLoadOp(inLoadOp)
            .setStoreOp(inStoreOp)
            .setImageLayout(inLayout)
            .setImageView(nullptr)
            .setClearValue(vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} })));
        
    if (myResolveImage)
    {
        myColorAttachments.Last()
            .setResolveImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
            .setResolveImageView(myResolveImage->GetImageView())
            .setResolveMode(vk::ResolveModeFlagBits::eAverage);
    }
        
    myColorFormats.Add(inFormat);
}

void GraphicsPass::AddDepthAttachment(
    VulkanImage* inImage, 
    vk::ImageLayout inLayout, 
    vk::AttachmentLoadOp inLoadOp,
    vk::AttachmentStoreOp inStoreOp, 
    VulkanImage* myDepthResolveImage)
{
    myHasDepthAttachment = true;
    myDepthAttachment
        .setLoadOp(inLoadOp)
        .setStoreOp(inStoreOp)
        .setImageLayout(inLayout)
        .setImageView(inImage->GetImageView())
        .setClearValue(vk::ClearDepthStencilValue(1.0f, 0u));
        
    RegisterAttachmentUsage(inImage, 
        vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests, 
        vk::AccessFlagBits::eDepthStencilAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentRead, 
        inLayout);
        
    if (myDepthResolveImage)
    {
        myDepthAttachment
            .setResolveImageLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
            .setResolveImageView(myDepthResolveImage->GetImageView())
            .setResolveMode(vk::ResolveModeFlagBits::eAverage);
            
        RegisterAttachmentUsage(myDepthResolveImage, 
            vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests, 
            vk::AccessFlagBits::eDepthStencilAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentRead, 
            inLayout);
    }
        
    myDepthFormat = inImage->GetFormat();
}

void GraphicsPass::DrawFromShadingBin(vk::CommandBuffer inCommandBuffer, const EShadingBin inShadingBin)
{
    uint maxNumDraws = Engine::GetEngineSystem<GPUSceneSystem>().GetNumObjects();
    
    ShadingBinHeader header;
    header.myShadingBin = inShadingBin;
    header.myElementsPerBin = maxNumDraws;
    inCommandBuffer.pushConstants(myPipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(ShadingBinHeader), &header);
    
    inCommandBuffer.drawIndexedIndirectCount(
        GPUResourceManager::Get()->GetBuffer<vk::DrawIndexedIndirectCommand>()->GetBuffer()->GetAPIResource(), 
        maxNumDraws * inShadingBin * sizeof(vk::DrawIndexedIndirectCommand),
        RenderSystem::Get()->myCountBuffer->GetAPIResource(), 
        inShadingBin * 4,
        maxNumDraws,
        sizeof(vk::DrawIndexedIndirectCommand));
}

void GraphicsPass::RegisterAttachmentUsage(VulkanImage* inImage, vk::PipelineStageFlags inPipelineStageFlags,
    vk::AccessFlags inAccessFlags, vk::ImageLayout inLayout)
{
    myAttachmentResourceUsages.Emplace().SetToImage(inImage, inPipelineStageFlags, inAccessFlags, inLayout);
}
