#pragma once
#include "Engine/ComponentSystem/Component.h"

class CharacterControllerComponent;

class PlayerComponent : public Component
{
public:
    PlayerComponent();
    
    void TickPhysics() override;

private:
    META(ExposeToEditor)
    float mySpeed = 5;
    
    META(ExposeToEditor)
    float mySprintSpeed = 10;
    
    META(ExposeToEditor)
    float myJumpForce = 500;
};
