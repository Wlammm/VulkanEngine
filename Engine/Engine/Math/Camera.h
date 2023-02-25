#pragma once

#include "Math/Vector2.hpp"
#include "Math/Matrix4x4.hpp"
#include "Math/Constants.hpp"

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

		myProjection(1, 1) = 2.f / inResolution.x;
		myProjection(2, 2) = 2.f / inResolution.y;
		myProjection(3, 3) = 1.f / (myFarPlane - myNearPlane);
		myProjection(4, 3) = myNearPlane / (myNearPlane - myFarPlane);
		myProjection(4, 4) = 1.f;
	}

	void CreatePerspective(const Vec2f& inResolution, const float inFov = 90.0f, const float inNearPlane = 1.0f, const float inFarPlane = 100000.0f)
	{
		myFov = inFov;
		myNearPlane = inNearPlane;
		myFarPlane = inFarPlane;

		myResolution = inResolution;

		const float hFoVRad = myFov * (PI / 180.0f);
		const float vFoVRad = 2.f * std::atan(std::tan(hFoVRad / 2.f) * (myResolution.y / myResolution.x));

		const float scaleX = 1.f / std::tan(hFoVRad * .5f);
		const float scaleY = 1.f / std::tan(vFoVRad * .5f);

		const float Q = myFarPlane / (myFarPlane - myNearPlane);

		myProjection(1, 1) = scaleX;
		myProjection(2, 2) = scaleY;
		myProjection(3, 3) = Q;
		myProjection(3, 4) = 1.f / Q;
		myProjection(4, 3) = -Q * myNearPlane;
		myProjection(4, 4) = 0.0f;
	}
};