#include "EnginePch.h"
#include "RenderGraph.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/Aftermath/NvidiaAftermathTracker.h"

void RenderGraph::DestroyRenderPasses()
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
    ZoneScoped;
    for (IRenderPass* pass : myPasses)
    {
        pass->PreExecute();

        InsertResourceBarriers(inCommandBuffer, pass->GetResourceUsage());
        InsertResourceBarriers(inCommandBuffer, pass->GetDynamicResourceUsages());

        if (VulkanContext::IsAftermathEnabled())
        {
            // Use app-managed marker resolution for stable, owned strings.
            VulkanContext::SetAftermathCheckpoint(inCommandBuffer, pass->GetName());
        }

        pass->Execute(inCommandBuffer);
    }
}

void RenderGraph::InsertResourceBarriers(vk::CommandBuffer inCommandBuffer, const List<ResourceUsage>& inResourceUsages)
{
    List<vk::ImageMemoryBarrier> imageBarriers;
    List<vk::BufferMemoryBarrier> bufferBarriers;
    List<vk::MemoryBarrier> memoryBarriers;

    // TODO(performance): These should be remembered from last passes use but for now this is fine.
    vk::PipelineStageFlags srcStageMask = vk::PipelineStageFlagBits::eTopOfPipe;
    vk::PipelineStageFlags dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;

    for (const ResourceUsage& usage : inResourceUsages)
    {
        if (usage.myImage)
        {
            HandleImageBarrier(usage, imageBarriers, srcStageMask, dstStageMask);
        }
        else if (usage.myBuffer)
        {
            HandleBufferBarrier(usage, bufferBarriers, srcStageMask, dstStageMask);
        }
        else if (usage.myAccelerationStructure)
        {
            HandleAccelerationStructureBarrier(usage, memoryBarriers, srcStageMask, dstStageMask);
        }
    }

    if (!imageBarriers.IsEmpty() || !bufferBarriers.IsEmpty() || !memoryBarriers.IsEmpty())
    {
        inCommandBuffer.pipelineBarrier(
            srcStageMask,
            dstStageMask,
            vk::DependencyFlags(),
            static_cast<uint32_t>(memoryBarriers.size()), memoryBarriers.data(),
            static_cast<uint32_t>(bufferBarriers.size()), bufferBarriers.data(),
            static_cast<uint32_t>(imageBarriers.size()), imageBarriers.data()
        );
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
            bool prevWasWrite = (currentState.myAccessFlags & (vk::AccessFlagBits::eMemoryWrite | vk::AccessFlagBits::eShaderWrite | vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite | vk::AccessFlagBits::eTransferWrite)) != vk::AccessFlagBits::eNone;
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
    ResourceState& currentState = myGlobalResourceState[inUsage.myBuffer->GetBuffer()];

    bool accessHazard = false;
    if (currentState.myAccessFlags != vk::AccessFlagBits::eNone)
    {
        bool prevWasWrite = (currentState.myAccessFlags & (vk::AccessFlagBits::eMemoryWrite | vk::AccessFlagBits::eShaderWrite | vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eTransferWrite | vk::AccessFlagBits::eAccelerationStructureWriteKHR)) != vk::AccessFlagBits::eNone;
        bool newIsWrite = !inUsage.myIsReadOnly;

        if (prevWasWrite || newIsWrite)
            accessHazard = true;
    }

    if (accessHazard)
    {
        vk::BufferMemoryBarrier barrier{};
        barrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
               .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
               .setBuffer(inUsage.myBuffer->GetBuffer()->GetAPIResource())
               .setSrcAccessMask(currentState.myAccessFlags)
               .setDstAccessMask(inUsage.myRequiredState.myAccessFlags)
               .setSize(VK_WHOLE_SIZE);

        outBarriers.Add(barrier);

        if (currentState.myStage == vk::PipelineStageFlags(0))
            currentState.myStage = vk::PipelineStageFlagBits::eTopOfPipe;

        inOutSrcGlobal |= currentState.myStage;
        inOutDstGlobal |= inUsage.myRequiredState.myStage;
    }

    currentState = inUsage.myRequiredState;
}

void RenderGraph::HandleAccelerationStructureBarrier(
    const ResourceUsage& inUsage,
    List<vk::MemoryBarrier>& outBarriers,
    vk::PipelineStageFlags& inOutSrcGlobal,
    vk::PipelineStageFlags& inOutDstGlobal)
{
    IGPUAccelerationStructure* accelerationStructure = inUsage.myAccelerationStructure;
    ResourceState& currentState = myGlobalResourceState[accelerationStructure];

    bool accessHazard = false;
    if (currentState.myAccessFlags != vk::AccessFlagBits::eNone)
    {
        bool prevWasWrite = (currentState.myAccessFlags & vk::AccessFlagBits::eAccelerationStructureWriteKHR) != vk::AccessFlagBits::eNone;
        bool newIsWrite   = !inUsage.myIsReadOnly;

        if (prevWasWrite || newIsWrite)
            accessHazard = true;
    }

    if (accessHazard)
    {
        vk::MemoryBarrier barrier{};
        barrier.setSrcAccessMask(currentState.myAccessFlags)
               .setDstAccessMask(inUsage.myRequiredState.myAccessFlags);

        outBarriers.Add(barrier);

        if (currentState.myStage == vk::PipelineStageFlags(0))
            currentState.myStage = vk::PipelineStageFlagBits::eTopOfPipe;

        inOutSrcGlobal |= currentState.myStage;
        inOutDstGlobal |= inUsage.myRequiredState.myStage;
    }

    currentState = inUsage.myRequiredState;
}
