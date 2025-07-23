#include "EditorPch.h"
#include "Viewport.h"

#include "EditorSystem/SelectionSystem.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanSwapChain.h"
#include "Engine/Vulkan/VulkanUtils.hpp"
#include "Engine/Vulkan/VulkanImage.h"
#include "Engine/Engine.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Rendering/RenderSystem.h"
#include "Engine/World/World.h"
#include "Engine/ComponentSystem/ComponentSystem.h"
#include "Engine/Components/EditorCameraMovementComponent.h"
#include "Engine/Components/StaticMeshComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Core/Input.h"
#include "Engine/Utils/Debug.h"
#include "Utils/ImGuiTextureUtils.h"

Viewport::Viewport()
	: EditorWindow("Viewport", true)
{
	mySampler = VulkanUtils::GetSampler(SamplerMode::Clamp);

	myDescriptorSets.Emplace();
	myDescriptorSets.Emplace();
	myDescriptorSets.Emplace();

	myPlayButtonTexture = ImGuiTextureUtils::CreateDescriptorSetForTexture("Editor/Viewport/PlayButton.png");
	myStopButtonTexture = ImGuiTextureUtils::CreateDescriptorSetForTexture("Editor/Viewport/StopButton.png");
	
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
	DrawPIEHUD();
	HandleDragDrop();
}

void Viewport::TickInput()
{
	EditorWindow::TickInput();
	UpdateCaptureMouse();
	CameraComponent* cameraComp = Engine::GetWorld()->GetMainCamera();
	EditorCameraMovementComponent* cameraMovComp = cameraComp->GetComponent<EditorCameraMovementComponent>();
	if (cameraMovComp)
		cameraMovComp->ViewportTick();
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
		World* world = Engine::GetWorld();
		ComponentSystem& componentSystem = world->GetComponentSystem();

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

void Viewport::DrawPIEHUD()
{
	const float hudWidth = 80.0f;
	const float hudHeight = 30.0f;
	const float padding = 8.0f;

	ImVec2 hudPos;
	hudPos.x = myP0.x + ((myP1.x - myP0.x) - hudWidth) * 0.5f;
	hudPos.y = myP0.y + padding;

	ImGui::SetNextWindowPos(hudPos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(hudWidth, hudHeight));
	ImGui::SetNextWindowBgAlpha(0.35f); 

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

	ImGuiWindowFlags windowFlags = 
		ImGuiWindowFlags_NoDecoration | 
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoNav | 
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing | 
		ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("##PIEOverlay", nullptr, windowFlags);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.1f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.2f));

	ImVec2 pos = ImGui::GetCursorPos();
	pos.y -= 5;
	ImGui::SetCursorPos(pos);
	
	if (ImGui::ImageButton(myPlayButtonTexture, {20, 20}))
	{
		if (!Editor::IsPIE())
			Editor::TogglePIE();
	}

	ImGui::SameLine();

	if (ImGui::ImageButton(myStopButtonTexture, {20, 20}))
	{
		if (Editor::IsPIE())
			Editor::TogglePIE();
	}
	
	ImGui::PopStyleColor(3);
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

void Viewport::HandleDragDrop()
{
	bool isPreviewingMeshThisFrame = false;
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::GetDragDropPayload();
		std::filesystem::path path = *(std::string*)payload->Data;

		if (ImGui::AcceptDragDropPayload(".fbx") || ImGui::AcceptDragDropPayload(".gltf"))
		{
			GameObject object = Engine::GetWorld()->GetComponentSystem().CreateGameObject(path.filename().string());
			object.AddComponent<StaticMeshComponent>(path);
			object.GetTransform()->SetPosition(myPreviewObject->GetTransform()->GetPosition());
			SelectionSystem::ClearSelection();
			SelectionSystem::SelectObject(object);
		}
		else
		{
			if (payload->IsDataType(".fbx") || payload->IsDataType(".gltf"))
			{
				isPreviewingMeshThisFrame = true;
				if (myPreviewObject == nullptr)
				{
					SpawnPreviewMesh(path);
				}

				UpdatePreviewMesh();
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (!isPreviewingMeshThisFrame && myPreviewObject != nullptr)
	{
		RemovePreviewMesh();
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

void Viewport::SpawnPreviewMesh(const std::filesystem::path& inMeshPath)
{
	myPreviewObject = new GameObject(Engine::GetWorld()->GetComponentSystem().CreateGameObject(inMeshPath.filename().string()));
	StaticMeshComponent* mesh = myPreviewObject->AddComponent<StaticMeshComponent>(inMeshPath);
	mesh->SetDepthWriteEnabled(false);
}

glm::vec2 Viewport::GetNormalizedMousePositionInViewport() const
{
	const ImVec2 imguiMousePos = ImGui::GetMousePos();
	
	const glm::vec2 imageSize = glm::vec2(myP1.x - myP0.x,myP1.y - myP0.y);

	glm::vec2 mousePos = glm::vec2(imguiMousePos.x, imguiMousePos.y);
	mousePos -= glm::vec2(myP0.x, myP0.y);
	mousePos /= imageSize;

	mousePos = glm::clamp(mousePos, glm::vec2(0, 0), glm::vec2(1, 1));
	
	return mousePos;
}

void Viewport::UpdatePreviewMesh()
{
	if (!myPreviewObject)
		return;
	
	CameraComponent* mainCamera = Engine::GetWorld()->GetMainCamera();
	check(mainCamera && "We're expecting a valid camera at all time in editor");
	
	const glm::vec2 normalizedScreenPos = GetNormalizedMousePositionInViewport();

	glm::vec3 worldPos;
	bool hit = mainCamera->ScreenToWorldPos(normalizedScreenPos, worldPos);
	if (!hit)
	{
		const glm::vec3 directionToObject = glm::normalize(mainCamera->GetRayDirectionFromScreen(normalizedScreenPos));
		worldPos = mainCamera->ScreenToNearPlane(normalizedScreenPos) + directionToObject * 5000.0f;
	}

	myPreviewObject->GetTransform()->SetPosition(worldPos); 
}

void Viewport::RemovePreviewMesh()
{
	myPreviewObject->Destroy();
	del(myPreviewObject);
}
