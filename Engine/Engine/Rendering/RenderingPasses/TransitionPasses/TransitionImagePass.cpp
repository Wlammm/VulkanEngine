#include "EnginePch.h"
#include "TransitionImagePass.h"

#include "Engine/Rendering/RenderSystem.h"
#include "Engine/Vulkan/VulkanImage.h"

TransitionImagePass::TransitionImagePass(
    VulkanImage* inImage, 
    vk::AccessFlagBits inSrcAccess, 
    vk::AccessFlagBits inDstAccess,
    vk::ImageLayout inSrcLayout, 
    vk::ImageLayout inDstLayout, 
    vk::PipelineStageFlagBits inSrcStage,
    vk::PipelineStageFlagBits inDstStage,
    vk::ImageAspectFlags inAspectFlags)
{
    myImage = inImage->GetAPIResource();
    mySrcAccess = inSrcAccess;
    myDstAccess = inDstAccess;
    mySrcLayout = inSrcLayout;
    myDstLayout = inDstLayout;
    mySrcStage = inSrcStage;
    myDstStage = inDstStage;
    myAspectFlags = inAspectFlags;
}

TransitionImagePass::TransitionImagePass(
    vk::Image inImage, 
    vk::AccessFlagBits inSrcAccess, 
    vk::AccessFlagBits inDstAccess,
    vk::ImageLayout inSrcLayout, 
    vk::ImageLayout inDstLayout, 
    vk::PipelineStageFlagBits inSrcStage,
    vk::PipelineStageFlagBits inDstStage,
    vk::ImageAspectFlags inAspectFlags)
{
    myImage = inImage;
    mySrcAccess = inSrcAccess;
    myDstAccess = inDstAccess;
    mySrcLayout = inSrcLayout;
    myDstLayout = inDstLayout;
    mySrcStage = inSrcStage;
    myDstStage = inDstStage;
    myAspectFlags = inAspectFlags;
}

void TransitionImagePass::CreateResources()
{
}

void TransitionImagePass::DestroyResources()
{
}

void TransitionImagePass::Execute(vk::CommandBuffer inCommandBuffer)
{
    GPUMARK_SCOPE(inCommandBuffer, "TransitionImagePass");
    
    vk::ImageMemoryBarrier barrier{
        mySrcAccess,   
        myDstAccess,            
        mySrcLayout,   
        myDstLayout,   
        VK_QUEUE_FAMILY_IGNORED,                
        VK_QUEUE_FAMILY_IGNORED,                  
        myImage,                              
        vk::ImageSubresourceRange{
            myAspectFlags,
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
