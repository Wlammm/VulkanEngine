#pragma once
#include "ComponentSystem/Component.h"

class EditorCameraMovementComponent : public Component
{
public:
    void Tick() override final;

private:
    static constexpr float myMouseSensitivity = 5.f;

    float myMovementSpeed = 10.0f;
    float myShiftMultiplier = 5.f;
    float myScrollMultiplier = 1.25f;
};
