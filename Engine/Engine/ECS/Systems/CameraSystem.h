#pragma once
#include "../System.h"
#include "Math/Transform.h"
#include "Math/Camera.h"

class CameraSystem : public System<Transform, const Camera>
{
public:
	CameraSystem();

	void Tick() override;

};