#include "EnginePch.h"
#include "IRenderPass.h"

void IRenderPass::BindBuffer(
    class IGPUBuffer* inBuffer, 
    vk::ShaderStageFlags inShaderStages, 
    uint inBindingIndex,
    vk::DescriptorType inDescriptorType, 
    vk::AccessFlags inAccessFlags)
{
    myDescriptorSet.BindBuffer(inBuffer, inShaderStages, inBindingIndex, inDescriptorType);
    RegisterBufferUsage(inBuffer, PipelineFlagsFromShaderStages(inShaderStages), inAccessFlags);
}

void IRenderPass::BindSampler(vk::Sampler inSampler, vk::ShaderStageFlags inShaderStages, uint inBindingIndex)
{
    myDescriptorSet.BindSampler(inSampler, inShaderStages, inBindingIndex);
}

void IRenderPass::BindImage(
    class VulkanImage* inImage, 
    const vk::Sampler inSampler, 
    const uint inBinding,
    const vk::ShaderStageFlags inShaderFlags, 
    const vk::ImageLayout inImageLayout, 
    vk::AccessFlags inAccessFlags)
{
    myDescriptorSet.BindImage(inImage, inSampler, inBinding, inShaderFlags, inImageLayout);
    RegisterImageUsage(inImage, PipelineFlagsFromShaderStages(inShaderFlags), inAccessFlags, inImageLayout);
}

void IRenderPass::Build()
{
    myDescriptorSet.Build();
}

void IRenderPass::BuildDescriptors(const List<SharedPtr<Shader>>& inShaders)
{
    
}

void IRenderPass::RegisterDynamicImageUsage(
    VulkanImage* inImage, 
    vk::PipelineStageFlags inStageFlags,
    vk::AccessFlags inAccess, 
    vk::ImageLayout inLayout)
{
    ResourceUsage& usage = myDynamicResourceUsages.Emplace();
    usage.SetToImage(inImage, inStageFlags, inAccess, inLayout);
}

void IRenderPass::RegisterImageUsage(
    VulkanImage* inImage, 
    vk::PipelineStageFlags inStageFlags,
    vk::AccessFlags inAccess, 
    vk::ImageLayout inLayout)
{
    ResourceUsage& usage = myResourceUsages.Emplace();
    usage.SetToImage(inImage, inStageFlags, inAccess, inLayout);    
}

void IRenderPass::RegisterBufferUsage(
    IGPUBuffer* inBuffer, 
    vk::PipelineStageFlags inStages,
    vk::AccessFlags inAccessFlags)
{
    ResourceUsage& usage = myResourceUsages.Emplace();
    usage.SetToBuffer(inBuffer, inStages, inAccessFlags);
}

vk::PipelineStageFlags IRenderPass::PipelineFlagsFromShaderStages(vk::ShaderStageFlags inShaderStages)
{
    vk::PipelineStageFlags flags;
        
    if (inShaderStages & vk::ShaderStageFlagBits::eVertex)
        flags |= vk::PipelineStageFlagBits::eVertexShader;
        
    if (inShaderStages & vk::ShaderStageFlagBits::eFragment)
        flags |= vk::PipelineStageFlagBits::eFragmentShader;
        
    if (inShaderStages & vk::ShaderStageFlagBits::eGeometry)
        flags |= vk::PipelineStageFlagBits::eGeometryShader;
        
    if (inShaderStages & vk::ShaderStageFlagBits::eCompute)
        flags |= vk::PipelineStageFlagBits::eComputeShader;
        
    return flags;
}