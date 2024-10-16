#include "EditorPch.h"
#include "Viewport.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanSwapChain.h"
#include "Engine/Vulkan/VulkanUtils.hpp"
#include "Engine/Vulkan/VulkanImage.h"
#include "Engine/Engine.h"
#include "Engine/Rendering/RenderSystem.h"
#include "Engine/World/World.h"
#include "Engine/ComponentSystem/ComponentSystem.h"
#include "Engine/Components/EditorCameraMovementComponent.h"

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
	UpdateCaptureMouse();
}

vk::DescriptorSet Viewport::GetCurrentDescriptorSet()
{
	return myDescriptorSets[VulkanContext::GetSwapChain().GetSyncIndex()];
}

void Viewport::UpdateCurrentTexture()
{
	const uint currentIndex = VulkanContext::GetSwapChain().GetSyncIndex();
	RenderSystem& renderSystem = Engine::GetEngineSystem<RenderSystem>();

	// This is probably incorrectly setup and only requires one descriptor set for all of them.
	ImGui_ImplVulkan_RemoveTexture(myDescriptorSets[currentIndex]);
	myDescriptorSets[currentIndex] = ImGui_ImplVulkan_AddTexture(mySampler, renderSystem.GetResolvedRenderTexture()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void Viewport::UpdateViewportImageSize()
{
	constexpr ImVec2 viewportOffset = ImVec2(8, 18);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	ImVec2 size = ClampToAspectRatio(viewportPanelSize, ImVec2(16, 9));

	myP0 = { (viewportPanelSize.x - size.x) * 0.5f, (viewportPanelSize.y - size.y) * 0.5f };

	myP0.x += viewportOffset.x;
	myP0.y += viewportOffset.y + 8.0f;
	
	ImGui::SetCursorPos(myP0);
	ImGui::Image(GetCurrentDescriptorSet(), size);

	ImVec2 windowPos = ImGui::GetWindowPos();
	myP0.x += windowPos.x;
	myP0.y += windowPos.y;

	myP1 = { myP0.x + size.x, myP0.y + size.y };
}

void Viewport::UpdateCaptureMouse()
{
	bool isMovingMouse = ImGui::GetIO().MouseDown[ImGuiMouseButton_Right];
	if (!isMovingMouse)
		return;

	if (myEditorCamera == nullptr)
	{
		World& world = Engine::GetWorld();
		ComponentSystem& componentSystem = world.GetComponentSystem();

		if (!(myEditorCamera = componentSystem.GetAnyComponentOfType<EditorCameraMovementComponent>()))
			return;
	}

	ImVec2 mousePosition = ImGui::GetIO().MousePos;

	if (mousePosition.x > myP1.x - 5.0f)
	{
		SetCursorPos(static_cast<int>(myP0.x + 10.0f), static_cast<int>(mousePosition.y));
		myEditorCamera->ResetMouseDelta();
	}
	else if (mousePosition.x < myP0.x + 5.0f)
	{
		SetCursorPos(static_cast<int>(myP1.x - 10.0f), static_cast<int>(mousePosition.y));
		myEditorCamera->ResetMouseDelta();
	}

	if (mousePosition.y > myP1.y - 5.0f)
	{
		SetCursorPos(static_cast<int>(mousePosition.x), static_cast<int>(myP0.y + 10.0f + 18.0f * 2.0f));
		myEditorCamera->ResetMouseDelta();
	}
	else if (mousePosition.y < myP0.y + 5.0f)
	{
		SetCursorPos(static_cast<int>(mousePosition.x), static_cast<int>(myP1.y - 10.0f));
		myEditorCamera->ResetMouseDelta();
	}
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