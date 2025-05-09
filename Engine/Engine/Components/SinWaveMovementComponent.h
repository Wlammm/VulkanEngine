#pragma once
#include "Engine/ComponentSystem/Component.h"

class SinWaveMovementComponent : public Component
{
public:
    void Tick() override final;
};
