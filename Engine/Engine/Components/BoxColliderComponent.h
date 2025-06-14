#pragma once

#include "ColliderComponent.h"

class BoxColliderComponent : public ColliderComponent
{
public:
    ~BoxColliderComponent();
    
    void OnCreate() override;

    void SetHalfSize(const glm::vec3& inHalfSize);
    
private:
    void OnScaleChanged() override;
    
private:
    glm::vec3 myHalfSize = glm::vec3(50.0f, 50.0f, 50.0f);
};
