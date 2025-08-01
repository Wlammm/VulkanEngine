#pragma once
#include "Engine/ComponentSystem/Component.h"

class PlayerCameraControllerComponent : public Component
{
public:
    void Tick() override;

private:
    META(SerializeField)
    float myMouseSensitivity = 15.0f;

    META(SerializeField)
    float mySpringArmChangeAmount = 20;
    
    META(SerializeField)
    float myMinSpringArmLength = 50;

    META(SerializeField)
    float myMaxSpringArmLength = 1000;

    float myPitch = 0.f;
    float myYaw = 0.f;

    float myStoredPitch = 0;
    float myStoredYaw = 0;
    float myStoredSpringArmLength = 0;
};
