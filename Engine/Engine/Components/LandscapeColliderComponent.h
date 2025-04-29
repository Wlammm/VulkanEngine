#pragma once
#include "ColliderComponent.h"
#include "Rendering/Vertex.hpp"

// Depends on the render component to get heightfield from.
class LandscapeColliderComponent : public ColliderComponent
{
public:
    void OnCreate() override;

    void Tick() override;

    void OnScaleChanged() override;
    
private:
    void OnComponentAdded(Component* inComponent);
    
    // Called whenever a landscape render component is added or during on create if one already exists. 
    void CreateCollider();
};
