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
    META(SerializeField)
    float myRadius = 50.0f;

    META(SerializeField)
    float myHeight = 100.0f;
};
