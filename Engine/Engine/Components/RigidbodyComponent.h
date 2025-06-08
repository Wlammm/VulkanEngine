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
    RigidbodyComponent();
    
    void TickPhysics() override;
    
    void OnCreate() override;
    void OnDestroy() override;

    void OnPhysicsStateDirty() override;

    // This should only be called during TickPhysics.
    void SetVelocity(const glm::vec3& inVelocity);
    
    // This should only be called during TickPhysics.
    glm::vec3 GetVelocity() const;
    
    void AttachCollider(ColliderComponent* inCollider);
    void DetachCollider(ColliderComponent* inCollider);

    void SetRotationConstraint(const bool inX, const bool inY, const bool inZ);
    
private:
    physx::PxRigidDynamic* myActor = nullptr;
    int myFramesSinceStartSleep = 0;
};
