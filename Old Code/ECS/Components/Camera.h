#pragma once

struct Camera 
{
	glm::vec2 myResolution;
	glm::mat4 myProjection;

	float myFov = 90.0f;
	float myNearPlane = 1.0f;
	float myFarPlane = 100000.0f;

	bool myIsOrthographic = false;

	void CreateOrthographic(const glm::vec2& inResolution, const float inNearPlane = 1.0f, const float inFarPlane = 100000.0f)
	{
		myNearPlane = inNearPlane;
		myFarPlane = inFarPlane;
		myResolution = inResolution;

		myProjection = glm::ortho(inResolution.x * -0.5f, inResolution.x * 0.5f, inResolution.y * -0.5f, inResolution.y * 0.5f, inNearPlane, inFarPlane);
		myProjection[1][1] *= -1;
		myIsOrthographic = true;
	}

	void CreatePerspective(const glm::vec2& inResolution, const float inFov = 90.0f, const float inNearPlane = 1.0f, const float inFarPlane = 100000.0f)
	{
		myFov = inFov;
		myNearPlane = inNearPlane;
		myFarPlane = inFarPlane;
		myResolution = inResolution;

		const float aspectRatio = inResolution.x / inResolution.y;
		myProjection = glm::perspective(glm::radians(inFov / aspectRatio), aspectRatio, inNearPlane, inFarPlane);
		myProjection[1][1] *= -1;
		myIsOrthographic = false;
	}
};