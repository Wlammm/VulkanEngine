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
	UpdateViewportImageSize();
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

void Viewport::UpdateViewportImageSize()
{
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	ImVec2 viewportOffset = ImVec2(8, 18);

	ImVec2 size = ClampToAspectRatio(viewportPanelSize, ImVec2(16, 9));

	ImVec2 p0 = { (viewportPanelSize.x - size.x) * 0.5f, (viewportPanelSize.y - size.y) * 0.5f };

	p0.x += viewportOffset.x;
	p0.y += viewportOffset.y + 8.0f;

	ImGui::SetCursorPos(p0);
	ImGui::Image(GetCurrentDescriptorSet(), size);
}

ImVec2 Viewport::ClampToAspectRatio(const ImVec2& inSize, const ImVec2& inAspectRatio) const
{
	float x = inSize.x, y = inSize.y;

	if (inSize.x < inSize.y * (inAspectRatio.x / inAspectRatio.y))
	{
		y = inSize.x * (inAspectRatio.y / inAspectRatio.x);
		x = inSize.x;
	}
	else if (inSize.y < inSize.x * (inAspectRatio.y / inAspectRatio.x))
	{
		y = inSize.y;
		x = inSize.y * (inAspectRatio.x / inAspectRatio.y);
	}
	else
	{
		y = inSize.y;
		x = inSize.x;
	}
	return ImVec2(x, y);
}
