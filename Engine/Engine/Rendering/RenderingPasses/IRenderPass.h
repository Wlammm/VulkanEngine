#pragma once
#include "Engine/Rendering/RenderGraph/RenderGraphTypes.hpp"
#include "Engine/Vulkan/VulkanDescriptorSet.h"

class VulkanImage;

class IRenderPass
{
public:
    virtual ~IRenderPass() = default;
    virtual void CreateResources() = 0;
    virtual void DestroyResources() = 0;
    
    virtual void Execute(vk::CommandBuffer inCommandBuffer) = 0;
    
    virtual void PreExecute() {}
    
    const List<ResourceUsage>& GetResourceUsage() const
    {
        return myResourceUsages;
    }
    
    const List<ResourceUsage>& GetDynamicResourceUsages() const
    {
        return myDynamicResourceUsages;
    }
    
    void BindBuffer(class IGPUBuffer* inBuffer, vk::ShaderStageFlags inShaderStages, uint inBindingIndex, vk::DescriptorType inDescriptorType, vk::AccessFlags inAccessFlags = vk::AccessFlagBits::eShaderRead)
    {
        myDescriptorSet.BindBuffer(inBuffer, inShaderStages, inBindingIndex, inDescriptorType);
        RegisterBufferUsage(inBuffer, PipelineFlagsFromShaderStages(inShaderStages), inAccessFlags);
    }
    
    void BindSampler(vk::Sampler inSampler, vk::ShaderStageFlags inShaderStages, uint inBindingIndex)
    {
        myDescriptorSet.BindSampler(inSampler, inShaderStages, inBindingIndex);
    }
    
    void BindImage(class VulkanImage* inImage, const vk::Sampler inSampler, const uint inBinding, const vk::ShaderStageFlags inShaderFlags, const vk::ImageLayout inImageLayout = vk::ImageLayout::eReadOnlyOptimal, vk::AccessFlags inAccessFlags = vk::AccessFlagBits::eShaderRead)
    {
        myDescriptorSet.BindImage(inImage, inSampler, inBinding, inShaderFlags, inImageLayout);
        RegisterImageUsage(inImage, PipelineFlagsFromShaderStages(inShaderFlags), inAccessFlags, inImageLayout);
    }
    
    void Build()
    {
        myDescriptorSet.Build();
    }
    
protected:
    void RegisterDynamicImageUsage(VulkanImage* inImage, vk::PipelineStageFlags inStageFlags, vk::AccessFlags inAccess, vk::ImageLayout inLayout)
    {
        ResourceUsage& usage = myDynamicResourceUsages.Emplace();
        usage.SetToImage(inImage, inStageFlags, inAccess, inLayout);
    }
    
    void RegisterImageUsage(VulkanImage* inImage, vk::PipelineStageFlags inStageFlags, vk::AccessFlags inAccess, vk::ImageLayout inLayout)
    {
        ResourceUsage& usage = myResourceUsages.Emplace();
        usage.SetToImage(inImage, inStageFlags, inAccess, inLayout);    
    }
    
    void RegisterBufferUsage(IGPUBuffer* inBuffer, vk::PipelineStageFlags inStages, vk::AccessFlags inAccessFlags)
    {
        ResourceUsage& usage = myResourceUsages.Emplace();
        usage.SetToBuffer(inBuffer, inStages, inAccessFlags);
    }
    
private:
    vk::PipelineStageFlags PipelineFlagsFromShaderStages(vk::ShaderStageFlags inShaderStages)
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
    
protected:
    VulkanDescriptorSet myDescriptorSet;
    
private:
    List<ResourceUsage> myResourceUsages;
    List<ResourceUsage> myDynamicResourceUsages;
};
