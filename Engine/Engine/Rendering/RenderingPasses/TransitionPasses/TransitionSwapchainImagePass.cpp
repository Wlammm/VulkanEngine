#include "EnginePch.h"
#include "TransitionSwapchainImagePass.h"

#include "Engine/Rendering/RenderSystem.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanImage.h"
#include "Engine/Vulkan/VulkanSwapChain.h"

TransitionSwapchainImagePass::TransitionSwapchainImagePass(
    vk::AccessFlagBits inSrcAccess, 
    vk::AccessFlagBits inDstAccess,
    vk::ImageLayout inSrcLayout, 
    vk::ImageLayout inDstLayout, 
    vk::PipelineStageFlagBits inSrcStage,
    vk::PipelineStageFlagBits inDstStage)
{
    mySrcAccess = inSrcAccess;
    myDstAccess = inDstAccess;
    mySrcLayout = inSrcLayout;
    myDstLayout = inDstLayout;
    mySrcStage = inSrcStage;
    myDstStage = inDstStage;
}

void TransitionSwapchainImagePass::CreateResources()
{
}

void TransitionSwapchainImagePass::DestroyResources()
{
}

void TransitionSwapchainImagePass::Execute(vk::CommandBuffer inCommandBuffer)
{
    GPUMARK_SCOPE(inCommandBuffer, "TransitionSwapchainImagePass");

    vk::ImageMemoryBarrier barrier{
        mySrcAccess,   
        myDstAccess,            
        mySrcLayout,   
        myDstLayout,   
        VK_QUEUE_FAMILY_IGNORED,                
        VK_QUEUE_FAMILY_IGNORED,                  
        VulkanContext::GetSwapChain().GetImage()->GetAPIResource(),                              
        vk::ImageSubresourceRange{
            vk::ImageAspectFlagBits::eColor,
            0, 1,                                   
            0, 1 
        }
    };

    inCommandBuffer.pipelineBarrier(
        mySrcStage, 
        myDstStage,
        {}, 
        nullptr,
        nullptr,
        barrier
    );
}
