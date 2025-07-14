#pragma once
#include "ColliderComponent.h"
#include "Engine/Rendering/Vertex.hpp"

// Depends on the render component to get heightfield from.
class LandscapeColliderComponent : public ColliderComponent
{
public:
    ~LandscapeColliderComponent();
    
    void OnCreate() override;

    void OnScaleChanged() override;
    
private:
    void OnComponentAdded(Component* inComponent);
    
    // Called whenever a landscape render component is added or during on create if one already exists. 
    void CreateCollider();

    bool myHasBoundDelegate = false;
};
