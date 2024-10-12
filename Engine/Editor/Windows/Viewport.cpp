#include "EditorPch.h"
#include "Viewport.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanSwapChain.h"
#include "Engine/Vulkan/VulkanUtils.hpp"
#include "Engine/Vulkan/VulkanImage.h"
#include "Engine/Engine.h"
#include "Engine/Rendering/RenderSystem.h"

Viewport::Viewport()
	: EditorWindow("Viewport", false)
{
	mySampler = VulkanUtils::GetSampler(SamplerMode::Clamp);

	myDescriptorSets.Emplace();
	myDescriptorSets.Emplace();
	myDescriptorSets.Emplace();
}

Viewport::~Viewport()
{
	LOG_WARNING("Viewport::~Viewport waits device idle");
	VulkanContext::GetDevice()->waitIdle();

	for(uint i  = 0; i < 3; ++i)
	{
		if (myDescriptorSets[i])
		{
			ImGui_ImplVulkan_RemoveTexture(myDescriptorSets[i]);
			myDescriptorSets[i] = nullptr;
		}
	}
}

void Viewport::Tick()
{
	UpdateCurrentTexture();
	ImVec2 size = { static_cast<float>(Engine::GetRenderResolution().x), static_cast<float>(Engine::GetRenderResolution().y) };
	ImGui::Image(GetCurrentDescriptorSet(), size);
}

vk::DescriptorSet Viewport::GetCurrentDescriptorSet()
{
	return myDescriptorSets[VulkanContext::GetSwapChain().GetSyncIndex()];
}

void Viewport::UpdateCurrentTexture()
{
	const uint currentIndex = VulkanContext::GetSwapChain().GetSyncIndex();
	RenderSystem& renderSystem = Engine::GetEngineSystem<RenderSystem>();
	
	ImGui_ImplVulkan_RemoveTexture(myDescriptorSets[currentIndex]);
	myDescriptorSets[currentIndex] = ImGui_ImplVulkan_AddTexture(mySampler, renderSystem.GetRenderTexture()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}
