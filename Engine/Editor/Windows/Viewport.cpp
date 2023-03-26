#include "EditorPch.h"
#include "Viewport.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/VUlkan/VulkanSwapChain.h"
#include "Engine/Vulkan/VulkanUtils.hpp"
#include "Engine/Engine.h"

Viewport::Viewport()
	: EditorWindow("Viewport", false)
{
	CreateDescriptorSets();
}

void Viewport::Tick()
{
	ImVec2 size = { static_cast<float>(Engine::GetRenderResolution().x), static_cast<float>(Engine::GetRenderResolution().y) };
	ImGui::Image(GetCurrentDescriptorSet(), size);
}

void Viewport::CreateDescriptorSets()
{
	mySampler = VulkanUtils::CreateSampler(SamplerMode::Clamp);

	for(uint i = 0; i < VulkanContext::GetSwapChain().GetFrameLag(); ++i)
	{
		auto& ds = myDescriptorSets.Add();
		ds = ImGui_ImplVulkan_AddTexture(mySampler, VulkanContext::GetSwapChain().GetImageView(i), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}
}

vk::DescriptorSet Viewport::GetCurrentDescriptorSet()
{
	return myDescriptorSets[VulkanContext::GetSwapChain().GetSwapChainIndex()];
}
