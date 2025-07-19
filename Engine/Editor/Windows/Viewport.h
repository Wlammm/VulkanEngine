#pragma once
#include "EditorWindow.h"
#include "Engine/Events/EventObserver.h"

class GameObject;
class EditorCameraMovementComponent;

class Viewport : public EditorWindow, public EventObserver
{
public:
	Viewport();
	~Viewport();

	virtual void Tick() override final;

	glm::vec2 GetNormalizedMousePositionInViewport() const;

private:
	//void OnSwapchainResized();

private:
	//void DestroyDescriptorSet();
	//void CreateDescriptorSet();

	vk::DescriptorSet GetCurrentDescriptorSet();

	void UpdateCurrentTexture();
	void UpdateViewportImageSize();
	void UpdateCaptureMouse();

	void DrawPIEHUD();
	void HandleDragDrop();

	ImVec2 ClampToAspectRatio(const ImVec2& inSize, const ImVec2& inAspectRatio) const;

	void SpawnPreviewMesh(const std::filesystem::path& inMeshPath);
	void UpdatePreviewMesh();
	void RemovePreviewMesh();
	
private:
	GameObject* myPreviewObject = nullptr;
	
	EditorCameraMovementComponent* myEditorCamera = nullptr;

	vk::DescriptorSet myPlayButtonTexture;
	vk::DescriptorSet myStopButtonTexture;

	List<vk::DescriptorSet> myDescriptorSets;
	vk::Sampler mySampler;
	
	// image size
	ImVec2 myP0;
	ImVec2 myP1;
};