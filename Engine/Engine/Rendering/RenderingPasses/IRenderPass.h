#pragma once
#include "Engine/Rendering/RenderGraph/RenderGraphTypes.hpp"
#include "Engine/Vulkan/Containers/IGPUAccelerationStructure.hpp"
#include "Engine/Vulkan/VulkanDescriptorSet.h"

class Shader;
class VulkanImage;

class IRenderPass
{
public:
    virtual ~IRenderPass() = default;
    virtual void CreateResources() = 0;
    virtual void DestroyResources() = 0;
    
    virtual void Execute(vk::CommandBuffer inCommandBuffer) = 0;
    
    virtual void PreExecute();

    virtual const char* GetName() const { return ""; }
    
    const List<ResourceUsage>& GetResourceUsage() const
    {
        return myResourceUsages;
    }
    
    const List<ResourceUsage>& GetDynamicResourceUsages() const
    {
        return myDynamicResourceUsages;
    }
    
    void BindBuffer(
        class IGPUBuffer* inBuffer, 
        vk::ShaderStageFlags inShaderStages, 
        uint inBindingIndex,
        vk::DescriptorType inDescriptorType,
        vk::AccessFlags inAccessFlags = vk::AccessFlagBits::eShaderRead);
    
    void BindSampler(vk::Sampler inSampler, vk::ShaderStageFlags inShaderStages, uint inBindingIndex);

    void BindAccelerationStructure(IGPUAccelerationStructure* inAS, vk::ShaderStageFlags inShaderStages, uint inBindingIndex,vk::AccessFlags inAccessFlags);
    
    void BindImage(
        class VulkanImage* inImage, 
        const vk::Sampler inSampler, 
        const uint inBinding, 
        const vk::ShaderStageFlags inShaderFlags, 
        const vk::ImageLayout inImageLayout = vk::ImageLayout::eReadOnlyOptimal, 
        vk::AccessFlags inAccessFlags = vk::AccessFlagBits::eShaderRead);
    
    void Build();

    void BuildDescriptors(const List<SharedPtr<Shader>>& inShaders);

    // Called from BuildDescriptors after auto-binding. Override to bind resources
    // that cannot be resolved automatically (e.g. buffers not in GPUResourceManager).
    virtual void SetupDescriptors() {}
    
protected:
    void RegisterDynamicImageUsage(VulkanImage* inImage, vk::PipelineStageFlags inStageFlags, vk::AccessFlags inAccess, vk::ImageLayout inLayout);
    
    void RegisterImageUsage(VulkanImage* inImage, vk::PipelineStageFlags inStageFlags, vk::AccessFlags inAccess, vk::ImageLayout inLayout);
    
    void RegisterBufferUsage(IGPUBuffer* inBuffer, vk::PipelineStageFlags inStages, vk::AccessFlags inAccessFlags);
    
    void RegisterAccelerationStructureUsage(IGPUAccelerationStructure* inAS, vk::PipelineStageFlags inStages, vk::AccessFlags inAccessFlags);
    
private:
    vk::PipelineStageFlags PipelineFlagsFromShaderStages(vk::ShaderStageFlags inShaderStages);

protected:
    VulkanDescriptorSet myDescriptorSet;
    vk::PushConstantRange myPushConstantRange{};

private:
    struct BoundAccelerationStructure
    {
        const IGPUAccelerationStructure* myAS;
        vk::ShaderStageFlags myShaderStages;
    };

    List<ResourceUsage> myResourceUsages;
    List<ResourceUsage> myDynamicResourceUsages;
};
