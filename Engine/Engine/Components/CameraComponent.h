#pragma once
#include "Engine/ComponentSystem/Component.h"

class CameraComponent : public Component
{
public:
    CameraComponent();

	const glm::mat4& GetProjection() const;
	
	void CreateOrthographic(const glm::vec2& inResolution, const float inNearPlane = 1.0f, const float inFarPlane = 100000.0f);
	void CreatePerspective(const glm::vec2& inResolution, const float inFov = 90.0f, const float inNearPlane = 1.0f, const float inFarPlane = 100000.0f);

	// Rebuilds the projection for a new render resolution, preserving the current fov/near/far and projection type.
	void SetResolution(const glm::vec2& inResolution);
	const glm::vec2& GetResolution() const;

	void SetAsMainCamera();
	
    bool ScreenToWorldPos(const glm::vec2& inNormalizedScreenPos, glm::vec3& outWorldPos) const;
	glm::vec3 ScreenToNearPlane(const glm::vec2& inNormalizedScreenPos) const;
	glm::vec3 ScreenToFarPlane(const glm::vec2& inNormalizedScreenPos) const;
	glm::vec3 GetRayDirectionFromScreen(const glm::vec2& inNormalizedScreenPos) const;

	float ToLinearDepth(const float inNonLinearDepth) const;
	float ToNormalizedLinearDepth(const float inNDCDepth) const;
private:
	glm::vec2 myResolution;
	glm::mat4 myProjection;

	META(SerializeField)
	float myFov = 90.0f;
	META(SerializeField)
	float myNearPlane = 1.0f;
	META(SerializeField)
	float myFarPlane = 100000.0f;

	bool myIsOrthographic = false;
};
