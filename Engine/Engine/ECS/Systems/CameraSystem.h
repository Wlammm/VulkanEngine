#pragma once
#include "../System.h"
#include "Math/Transform.h"
#include "Math/Camera.h"

class CameraSystem : public System<Transform, const Camera>
{
public:
	CameraSystem();

	void Tick() override final;

private:
	static constexpr float myMouseSensitivity = 5.f;

	float myMovementSpeed = 0.025f;
	float myShiftMultiplier = 5.f;
	float myScrollMultiplier = 1.25f;
};