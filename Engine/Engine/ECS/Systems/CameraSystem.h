#pragma once
#include "ECS/System.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Camera.h"

class CameraSystem : public System<Transform, const Camera>
{
public:
	CameraSystem();

	void Tick() override final;

private:
	static constexpr float myMouseSensitivity = 5.f;

	float myMovementSpeed = 2.0f;
	float myShiftMultiplier = 5.f;
	float myScrollMultiplier = 1.25f;
};