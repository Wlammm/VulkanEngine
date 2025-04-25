#pragma once
#include "ColliderComponent.h"
#include "Rendering/Vertex.hpp"

class LandscapeColliderComponent : public ColliderComponent
{
public:
    void OnCreate() override;

    void OnScaleChanged() override;
private:
};
