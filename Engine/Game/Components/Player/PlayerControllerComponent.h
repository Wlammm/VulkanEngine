#pragma once
#include "Engine/ComponentSystem/Component.h"

class PlayerControllerComponent : public Component
{
public:
    PlayerControllerComponent();

    void Tick() override;

private:
    float mySpeed = 100;
    
};
