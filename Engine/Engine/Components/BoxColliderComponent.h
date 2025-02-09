#pragma once
#include <PxShape.h>

#include "ColliderComponent.h"

class BoxColliderComponent : public ColliderComponent
{
public:
    BoxColliderComponent();
    ~BoxColliderComponent() override;

    void OnCreate() override;
    
private:
    glm::vec3 mySize = glm::vec3(100.0f, 100.0f, 100.0f);
};
