#pragma once
#include "Engine/Rendering/RenderingPasses/IRenderPass.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanImage.h"

class IRenderPass;

class RenderGraph
{
public:
    void DestroyRenderPasses();
    
    void AddPass(IRenderPass* inRenderPass);
    
    void Execute(vk::CommandBuffer inCommandBuffer);
    
    /*
     * Will insert resource barriers for the specified resource into the command list.
     */
    void InsertResourceBarriers(vk::CommandBuffer inCommandBuffer, const List<ResourceUsage>& inResourceUsages);
    
private:
    void HandleImageBarrier(const ResourceUsage& inUsage, List<vk::ImageMemoryBarrier>& outBarriers, vk::PipelineStageFlags& inOutSrcGlobal, vk::PipelineStageFlags& inOutDstGlobal);

    void HandleBufferBarrier(const ResourceUsage& inUsage, List<vk::BufferMemoryBarrier>& outBarriers, vk::PipelineStageFlags& inOutSrcGlobal, vk::PipelineStageFlags& inOutDstGlobal);

    void HandleAccelerationStructureBarrier(const ResourceUsage& inUsage, List<vk::MemoryBarrier>& outBarriers, vk::PipelineStageFlags& inOutSrcGlobal, vk::PipelineStageFlags& inOutDstGlobal);
    
private:
    List<IRenderPass*> myPasses;
    
    // TODO: This only grows. Whenever a resource is destroyed, it should be removed from here.
    std::map<void*, ResourceState> myGlobalResourceState;
};
