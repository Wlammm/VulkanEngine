#include "EnginePch.h"
#include "PresentPass.h"

#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanSwapChain.h"

PresentPass::PresentPass()
{
    
}

void PresentPass::CreateResources()
{
    
}

void PresentPass::DestroyResources()
{
}

void PresentPass::PreExecute()
{
    IRenderPass::PreExecute();
    RegisterDynamicImageUsage(VulkanContext::GetSwapChain().GetImage(), vk::PipelineStageFlagBits::eBottomOfPipe, vk::AccessFlagBits::eNone, vk::ImageLayout::ePresentSrcKHR);
}

void PresentPass::Execute(vk::CommandBuffer inCommandBuffer)
{
    
}
