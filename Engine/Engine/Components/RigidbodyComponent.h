#pragma once
#include "ComponentSystem/Component.h"

namespace physx
{
    class PxRigidDynamic;
}

class ColliderComponent;

class RigidbodyComponent : public Component
{
public:
    RigidbodyComponent();

    bool DoesComponentImplementPhysicsFunctions() const override { return true; }
    
    void TickPhysics() override;
    
    void OnCreate() override;
    void OnDestroy() override;

    void OnPhysicsStateDirty() override;
    
    void AttachCollider(ColliderComponent* inCollider);
    void DetachCollider(ColliderComponent* inCollider);
    
private:
    physx::PxRigidDynamic* myActor = nullptr;
};
