#pragma once
#include "RigidbodyComponent.h"
#include "ComponentSystem/Component.h"

namespace physx
{
    class PxRigidStatic;
    class PxShape;
}

class ColliderComponent : public Component
{
public:
    ColliderComponent();

    // Make sure to set the value of myShape before calling this.
    void OnCreate() override;

    physx::PxShape* GetShape() const;

    void OnRigidbodyCreated(RigidbodyComponent* inRigidbody);
    
protected:
    physx::PxShape* myShape = nullptr;
    physx::PxRigidStatic* myActor = nullptr;
};
