#pragma once
#include "ComponentSystem/Component.h"

class SinWaveMovementComponent : public Component
{
public:
    void Tick() override final;
};
