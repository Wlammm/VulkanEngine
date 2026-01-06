#pragma once
#include "Engine/Rendering/RenderingPasses/IRenderPass.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanImage.h"

class IRenderPass;

class RenderGraph
{
public:
    ~RenderGraph();
    
    void AddPass(IRenderPass* inRenderPass);
    
    void Execute(vk::CommandBuffer inCommandBuffer);
    
private:
    void HandleImageBarrier(const ResourceUsage& inUsage, List<vk::ImageMemoryBarrier>& outBarriers, vk::PipelineStageFlags& inOutSrcGlobal, vk::PipelineStageFlags& inOutDstGlobal);
    
    void HandleBufferBarrier(const ResourceUsage& inUsage, List<vk::BufferMemoryBarrier>& outBarriers, vk::PipelineStageFlags& inOutSrcGlobal, vk::PipelineStageFlags& inOutDstGlobal);
    
private:
    List<IRenderPass*> myPasses;
    std::map<void*, ResourceState> myGlobalResourceState;
};
