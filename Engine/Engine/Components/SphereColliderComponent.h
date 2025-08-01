#pragma once
#include "ColliderComponent.h"

class SphereColliderComponent : public ColliderComponent
{
public:
    void OnCreate() override;

    void SetRadius(const float inRadius);
    
private:
    void OnScaleChanged() override;
    
private:
    META(SerializeField)
    float myRadius = 50;
};
