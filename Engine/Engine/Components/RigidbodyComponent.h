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
    
    void AttachCollider(ColliderComponent* inCollider);
    void DetachCollider(ColliderComponent* inCollider);

    void SetRotationConstraint(const bool inX, const bool inY, const bool inZ);
    
private:
    physx::PxRigidDynamic* myActor = nullptr;
    int myFramesSinceStartSleep = 0;
};
