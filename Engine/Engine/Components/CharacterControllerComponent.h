#pragma once
#include <characterkinematic/PxController.h>

#include "Engine/ComponentSystem/Component.h"

class CharacterControllerComponent : public Component
{
    COMP(CharacterControllerComponent);
public:
    void OnCreate() override;
    void OnDestroy() override;

    void OnPositionChanged();

    void Move(const glm::vec3& inMovement);

    void TickPhysics() override;

    float GetHeight() const;
    void SetHeight(const float inHeight);

    float GetRadius() const;
    void SetRadius(const float inRadius);

    bool IsGrounded() const;

    void SetPositionOffset(const glm::vec3& inOffset);

    void Jump(const float inJumpForce);
    
private:
    physx::PxController* myController = nullptr;
    physx::PxControllerCollisionFlags myCollisionFlags;
	physx::PxControllerFilters myFilter = 0;

    float myHeight = 200;
    float myRadius = 20;
    float mySlopeLimitDegrees = 45;
    float myMinDist = 0.1f;
    float myStepOffset = 10.0f;
    bool myUseGravity = true;

    float myDownVelocity = 0;
    float myGravity = 981;
    float myTerminalVelocity = 5500;

    glm::vec3 myDisplacement = glm::vec3(0, 0, 0);
    glm::vec3 myPositionOffset = glm::vec3(0, 0, 0);
};
