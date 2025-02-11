#pragma once
#include "ColliderComponent.h"

class SphereColliderComponent : public ColliderComponent
{
public:
    void OnCreate() override;

private:
    float myRadius = 50;
};
