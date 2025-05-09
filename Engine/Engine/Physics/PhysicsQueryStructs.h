#pragma once

struct RaycastHit
{
    glm::vec3 myHitPosition;
    glm::vec3 myHitNormal;
    float myHitDistance;
    class GameObject* myHitGameObject;
};