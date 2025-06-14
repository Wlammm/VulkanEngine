#pragma once
#include "Engine/ComponentSystem/Component.h"

class PlayerCameraControllerComponent : public Component
{
public:
    void Tick() override;

private:
    float myMouseSensitivity = 15.0f;
    float mySpringArmChangeAmount = 20;
    float myMinSpringArmLength = 50;
    float myMaxSpringArmLength = 1000;

    float myPitch = 0.f;
    float myYaw = 0.f;

    float myStoredPitch = 0;
    float myStoredYaw = 0;
    float myStoredSpringArmLength = 0;
};
