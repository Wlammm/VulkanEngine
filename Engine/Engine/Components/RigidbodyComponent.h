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

    void TickPhysics() override;
    
    void OnCreate() override;
    
    void AttachCollider(ColliderComponent* inCollider);
    void DetachCollider(ColliderComponent* inCollider);
    
private:
    physx::PxRigidDynamic* myActor = nullptr;
};
