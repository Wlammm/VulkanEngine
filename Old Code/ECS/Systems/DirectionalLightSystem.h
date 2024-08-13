#pragma once
#include "ECS/System.h"


class DirectionalLightSystem : public System
{
public:
	DirectionalLightSystem();

	virtual void Tick() override final;
};