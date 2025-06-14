#pragma once
#include "Engine/ComponentSystem/Component.h"

class CharacterControllerComponent;

class PlayerComponent : public Component
{
public:
    PlayerComponent();
    
    void TickPhysics() override;

private:
    float mySpeed = 5;
    float mySprintSpeed = 10;

    float myJumpForce = 500;
};
