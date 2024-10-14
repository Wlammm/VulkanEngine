#pragma once
#include "EditorWindow.h"
#include "Engine/Events/EventObserver.h"

class EditorCameraMovementComponent;

class Viewport : public EditorWindow, public EventObserver
{
public:
	Viewport();
	~Viewport();

	virtual void Tick() override final;

private:
	//void OnSwapchainResized();

private:
	//void DestroyDescriptorSet();
	//void CreateDescriptorSet();

	vk::DescriptorSet GetCurrentDescriptorSet();

	void UpdateCurrentTexture();
	void UpdateViewportImageSize();
	void UpdateCaptureMouse();

	ImVec2 ClampToAspectRatio(const ImVec2& inSize, const ImVec2& inAspectRatio) const;

private:
	EditorCameraMovementComponent* myEditorCamera = nullptr;

	List<vk::DescriptorSet> myDescriptorSets;
	vk::Sampler mySampler;

	// image size
	ImVec2 myP0;
	ImVec2 myP1;
};