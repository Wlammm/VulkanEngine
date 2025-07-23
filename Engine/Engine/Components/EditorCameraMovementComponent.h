#pragma once
#include "Engine/ComponentSystem/Component.h"

class EditorCameraMovementComponent : public Component
{
public:
    void ViewportTick();

    void ResetMouseDelta();

private:
    void UpdateMovement();
    void UpdateRotation();

private:
    static constexpr float myMouseSensitivity = 5.f;

    bool myResetMouseDelta = false;
    glm::vec2 myMouseDelta;
    float myYaw = 0.0f;
    float myPitch = 0.0f;

    float myMovementSpeed = 10.0f;
    float myShiftMultiplier = 5.f;
    float myScrollMultiplier = 1.25f;
};
