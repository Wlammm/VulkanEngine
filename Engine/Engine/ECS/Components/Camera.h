#pragma once

struct Camera 
{
	Vec2f myResolution;
	Mat4f myProjection;

	float myFov = 90.0f;
	float myNearPlane = 1.0f;
	float myFarPlane = 100000.0f;

	void CreateOrthographic(const Vec2f& inResolution, const float inNearPlane = 1.0f, const float inFarPlane = 100000.0f)
	{
		myNearPlane = inNearPlane;
		myFarPlane = inFarPlane;
		myResolution = inResolution;

		myProjection = Mat4f::CreateOrthographic(inResolution, inNearPlane, inFarPlane);
	}

	void CreatePerspective(const Vec2f& inResolution, const float inFov = 90.0f, const float inNearPlane = 1.0f, const float inFarPlane = 100000.0f)
	{
		myFov = inFov;
		myNearPlane = inNearPlane;
		myFarPlane = inFarPlane;
		myResolution = inResolution;

		myProjection = Mat4f::CreatePerspective(inResolution, inFov, inNearPlane, inFarPlane);
	}
};