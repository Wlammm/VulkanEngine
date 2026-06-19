#pragma once
#include "Engine/Rendering/RenderGraph/RenderGraphTypes.hpp"
#include "Engine/Vulkan/Containers/IGPUAccelerationStructure.hpp"
#include "Engine/Vulkan/VulkanDescriptorSet.h"

class Shader;
class VulkanImage;
class IGPUBuffer;

class IRenderPass
{
public:
    virtual ~IRenderPass() = default;
    virtual void CreateResources() = 0;
    virtual void DestroyResources() = 0;
    
    virtual void Execute(vk::CommandBuffer inCommandBuffer) = 0;
    
    virtual void PreExecute();

    virtual const char* GetName() const { return ""; }
    
    const VulkanDescriptorSet& GetDescriptorSet() const;
    
    const List<ResourceUsage>& GetDynamicResourceUsages() const
    {
        return myDynamicResourceUsages;
    }

    // Barrier usages for buffers this pass consumes as indirect-draw command/count
    // sources. Rebuilt each frame from the registered buffers so that resize-recreated
    // buffers resolve to their current VulkanBuffer.
    List<ResourceUsage> GetIndirectDrawUsages() const;
    
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
    
    // Called from RenderGraph during setup.
    void SetRenderGraph(RenderGraph* inRenderGraph);
    
protected:
    RenderGraph* GetRenderGraph() const;
    
    void RegisterDynamicImageUsage(VulkanImage* inImage, vk::PipelineStageFlags inStageFlags, vk::AccessFlags inAccess, vk::ImageLayout inLayout);
    void RegisterDynamicBufferUsage(VulkanBuffer* inBuffer, vk::PipelineStageFlagBits inStageFlags, vk::AccessFlags inAccess);

    // Declare (once, during pass setup) a buffer this pass reads as an indirect-draw
    // source. The RenderGraph then inserts the indirect-read barrier automatically each
    // frame — there is no need to register it as a dynamic usage per frame.
    void RegisterIndirectDrawBuffer(IGPUBuffer* inBuffer);
    
protected:
    VulkanDescriptorSet myDescriptorSet;
    vk::PushConstantRange myPushConstantRange{};

private:
    struct BoundAccelerationStructure
    {
        const IGPUAccelerationStructure* myAS;
        vk::ShaderStageFlags myShaderStages;
    };

    List<ResourceUsage> myDynamicResourceUsages;
    List<IGPUBuffer*> myIndirectDrawBuffers;

    RenderGraph* myRenderGraph = nullptr;
};
