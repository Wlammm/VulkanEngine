#pragma once
#include "Engine/ComponentSystem/Component.h"

namespace physx
{
    class PxRigidDynamic;
}

class ColliderComponent;

class RigidbodyComponent : public Component
{
public:
    RigidbodyComponent() = default;
    
    void TickPhysics() override;
    
    void OnCreate() override;
    void OnDestroy() override;

    // This should only be called during TickPhysics.
    void SetVelocity(const glm::vec3& inVelocity);
    
    // This should only be called during TickPhysics.
    glm::vec3 GetVelocity() const;
    
    void AttachCollider(ColliderComponent* inCollider);
    void DetachCollider(ColliderComponent* inCollider);

    void SetRotationConstraint(const bool inX, const bool inY, const bool inZ);

private:
    void OnPositionChanged();
    void OnRotationChanged();
    
private:
    physx::PxRigidDynamic* myActor = nullptr;
    int myFramesSinceStartSleep = 0;
};
