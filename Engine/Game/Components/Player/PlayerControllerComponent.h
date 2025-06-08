#pragma once
#include "Engine/ComponentSystem/Component.h"

class PlayerControllerComponent : public Component
{
public:
    PlayerControllerComponent();
    
    void TickPhysics() override;

private:
    float mySpeed = 500;
    float mySprintSpeed = 1500;

    float myJumpForce = 1000;
};
