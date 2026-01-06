#include "EnginePch.h"
#include "RenderGraph.h"

RenderGraph::~RenderGraph()
{
    for (IRenderPass* pass : myPasses)
    {
        pass->DestroyResources();
        del(pass);
    }
    myPasses.Clear();
}

void RenderGraph::AddPass(IRenderPass* inRenderPass)
{
    inRenderPass->CreateResources();
    myPasses.Add(inRenderPass);
}

void RenderGraph::Execute(vk::CommandBuffer inCommandBuffer)
{
    for (IRenderPass* pass : myPasses)
    {
        pass->PreExecute();
        
        List<vk::ImageMemoryBarrier> imageBarriers;
        List<vk::BufferMemoryBarrier> bufferBarriers;
            
        // TODO(performance): These should be remembered from last passes use but for now this is fine.
        vk::PipelineStageFlags srcStageMask = vk::PipelineStageFlagBits::eTopOfPipe;
        vk::PipelineStageFlags dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
        
        for (const ResourceUsage& usage : pass->GetResourceUsage())
        {
            if (usage.myImage)
            {
                HandleImageBarrier(usage, imageBarriers, srcStageMask, dstStageMask);
            }
            else if (usage.myBuffer)
            {
                HandleBufferBarrier(usage, bufferBarriers, srcStageMask, dstStageMask);
            }
        }
        
        for (const ResourceUsage& usage : pass->GetDynamicResourceUsages())
        {
            if (usage.myImage) 
                HandleImageBarrier(usage, imageBarriers, srcStageMask, dstStageMask);
        }
            
        if (!imageBarriers.IsEmpty() || !bufferBarriers.IsEmpty())
        {
            // Optimization: You can refine stage masks here based on the collected barriers
            inCommandBuffer.pipelineBarrier(
                srcStageMask, // Calculated in Handle functions
                dstStageMask, 
                vk::DependencyFlags(),
                0, nullptr,
                static_cast<uint32_t>(bufferBarriers.size()), bufferBarriers.data(),
                static_cast<uint32_t>(imageBarriers.size()), imageBarriers.data()
            );
        }
        
        pass->Execute(inCommandBuffer);
    }
}

void RenderGraph::HandleImageBarrier(
    const ResourceUsage& inUsage, 
    List<vk::ImageMemoryBarrier>& outBarriers,
    vk::PipelineStageFlags& inOutSrcGlobal, 
    vk::PipelineStageFlags& inOutDstGlobal)
{
        ResourceState& currentState = myGlobalResourceState[inUsage.myImage];
        
        bool layoutMismatch = currentState.myLayout != inUsage.myRequiredState.myLayout;
        
        bool accessHazard = false;
        if (currentState.myAccessFlags != vk::AccessFlagBits::eNone) // If previously accessed
        {
            // Simplified hazard check: If either new or old was a write, we need a barrier.
            bool prevWasWrite = (currentState.myAccessFlags & (vk::AccessFlagBits::eMemoryWrite | vk::AccessFlagBits::eShaderWrite | vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eTransferWrite)) != vk::AccessFlagBits::eNone;
            bool newIsWrite = !inUsage.myIsReadOnly;
             
            if (prevWasWrite || newIsWrite) 
                accessHazard = true;
        }
        
        if (layoutMismatch || accessHazard)
        {
            vk::ImageMemoryBarrier barrier{};
            barrier.setOldLayout(currentState.myLayout)
                   .setNewLayout(inUsage.myRequiredState.myLayout)
                   .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED) // Future: currentState.queueFamily
                   .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED) // Future: inUsage.requiredState.queueFamily
                   .setImage(inUsage.myImage->GetAPIResource())
                   .setSubresourceRange(inUsage.myImage->GetSubresourceRange())
                   .setSrcAccessMask(currentState.myAccessFlags)
                   .setDstAccessMask(inUsage.myRequiredState.myAccessFlags);

            if (currentState.myLayout == vk::ImageLayout::eUndefined)
            {
                barrier.setSrcAccessMask(vk::AccessFlagBits::eNone);
            }

            outBarriers.Add(barrier);

            if(currentState.myStage == vk::PipelineStageFlags(0)) 
                currentState.myStage = vk::PipelineStageFlagBits::eTopOfPipe;
            
            inOutSrcGlobal |= currentState.myStage;
            inOutDstGlobal |= inUsage.myRequiredState.myStage;
            
            currentState = inUsage.myRequiredState;
        }
    }

void RenderGraph::HandleBufferBarrier(
    const ResourceUsage& inUsage, 
    List<vk::BufferMemoryBarrier>& outBarriers,
    vk::PipelineStageFlags& inOutSrcGlobal, 
    vk::PipelineStageFlags& inOutDstGlobal)
{
    ResourceState& currentState = myGlobalResourceState[inUsage.myBuffer];
    
    bool accessHazard = false;
    if (currentState.myAccessFlags != vk::AccessFlagBits::eNone) // If previously accessed
    {
        // Simplified hazard check: If either new or old was a write, we need a barrier.
        bool prevWasWrite = (currentState.myAccessFlags & (vk::AccessFlagBits::eMemoryWrite | vk::AccessFlagBits::eShaderWrite | vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eTransferWrite)) != vk::AccessFlagBits::eNone;
        bool newIsWrite = !inUsage.myIsReadOnly;
             
        if (prevWasWrite || newIsWrite) 
            accessHazard = true;
    }
        
    if (accessHazard)
    {
        vk::BufferMemoryBarrier barrier{};
        barrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED) // Future: currentState.queueFamily
               .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED) // Future: inUsage.requiredState.queueFamily
               .setBuffer(inUsage.myBuffer->GetBuffer()->GetAPIResource())
               .setSrcAccessMask(currentState.myAccessFlags)
               .setDstAccessMask(inUsage.myRequiredState.myAccessFlags)
               .setSize(VK_WHOLE_SIZE);

        outBarriers.Add(barrier);

        if(currentState.myStage == vk::PipelineStageFlags(0))
            currentState.myStage = vk::PipelineStageFlagBits::eTopOfPipe;
            
        inOutSrcGlobal |= currentState.myStage;
        inOutDstGlobal |= inUsage.myRequiredState.myStage;
    }
    
    currentState = inUsage.myRequiredState;
}
