#pragma once
#include "ECS/System.h"

class PointLightSystem : public System
{
public:
	PointLightSystem();

private:
	// Inherited via System
	virtual void Tick() override final;

};