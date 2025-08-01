#pragma once
#include "Engine/ComponentSystem/Component.h"

class CharacterControllerComponent;

class PlayerComponent : public Component
{
public:
    PlayerComponent();
    
    void TickPhysics() override;

private:
    META(SerializeField)
    float mySpeed = 5;
    
    META(SerializeField)
    float mySprintSpeed = 10;
    
    META(SerializeField)
    float myJumpForce = 500;
};
