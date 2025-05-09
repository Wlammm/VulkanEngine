#pragma once
#include "Engine/ComponentSystem/Component.h"

class CameraComponent : public Component
{
public:
    CameraComponent();

	const glm::mat4& GetProjection() const;
	
	void CreateOrthographic(const glm::vec2& inResolution, const float inNearPlane = 1.0f, const float inFarPlane = 100000.0f);
	void CreatePerspective(const glm::vec2& inResolution, const float inFov = 90.0f, const float inNearPlane = 1.0f, const float inFarPlane = 100000.0f);
	
private:
	glm::vec2 myResolution;
	glm::mat4 myProjection;

	float myFov = 90.0f;
	float myNearPlane = 1.0f;
	float myFarPlane = 100000.0f;

	bool myIsOrthographic = false;

};
