#pragma once

#include "ColliderComponent.h"

class BoxColliderComponent : public ColliderComponent
{
public:
    BoxColliderComponent();
    ~BoxColliderComponent() override;

    void OnCreate() override;

    void Tick() override;

private:
    glm::vec3 myHalfSize = glm::vec3(50.0f, 50.0f, 50.0f);
};
