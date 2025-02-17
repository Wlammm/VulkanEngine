#pragma once
#include "ColliderComponent.h"

class CapsuleColliderComponent : public ColliderComponent
{
public:
    void OnCreate() override;

    void SetRadius(const float inRadius);
    void SetHeight(const float inHeight);

private:
    void OnScaleChanged() override;
    
private:
    float myRadius = 50.0f;
    float myHeight = 100.0f;
};
