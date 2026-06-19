#pragma once
#include "EditorWindow.h"

class Texture;
class StaticMeshActor;
class GameObject;
class EditorCameraMovementComponent;

class Viewport : public EditorWindow
{
public:
	// How the rendered image is fit inside the viewport panel.
	enum class DisplayMode
	{
		AspectRatio16x9,	// Letterboxed to a fixed 16:9 aspect ratio.
		Fill,				// Fills the entire viewport panel.
	};

	Viewport();
	~Viewport();

	virtual void Tick() override final;
	void TickInput() override;

	glm::vec2 GetNormalizedMousePositionInViewport() const;
	
	const ImVec2& GetViewportPos() const;
	const ImVec2& GetViewportSize() const;

private:
	void TickEditorCamera();

private:
	//void DestroyDescriptorSet();
	//void CreateDescriptorSet();

	vk::DescriptorSet GetCurrentDescriptorSet();

	void UpdateCurrentTexture();
	void UpdateViewportImageSize();
	void UpdateCameraResolution();
	void UpdateCaptureMouse();

	void DrawPIEHUD();
	void DrawViewportSettings();
	void HandleDragDrop();

	ImVec2 ClampToAspectRatio(const ImVec2& inSize, const ImVec2& inAspectRatio) const;

	void SpawnPreviewMesh(const std::filesystem::path& inMeshPath);
	void UpdatePreviewMesh();
	void RemovePreviewMesh();
	
	
private:
	StaticMeshActor* myPreviewActor = nullptr;
	
	SharedPtr<Texture> myPlayButtonTexture;
	vk::DescriptorSet myPlayButtonDescriptor;
	SharedPtr<Texture> myStopButtonTexture;
	vk::DescriptorSet myStopButtonDescriptor;

	List<vk::DescriptorSet> myDescriptorSets;
	vk::Sampler mySampler;
	
	DisplayMode myDisplayMode = DisplayMode::AspectRatio16x9;

	// image size
	ImVec2 myP0;
	ImVec2 myP1;

	ImVec2 myViewportSize;
};