#pragma once
#include "ColliderComponent.h"
#include "Rendering/Vertex.hpp"

class LandscapeColliderComponent : public ColliderComponent
{
public:
    LandscapeColliderComponent();

    void OnCreate() override;
    
private:
};
