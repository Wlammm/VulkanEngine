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
    // Make sure to set the value of myShape before calling this.
    void OnCreate() override;

    void OnDestroy() override;

    void OnPhysicsStateDirty() override;
    
    physx::PxShape* GetShape() const;

    void SetIsTrigger(const bool inIsTrigger) const;

    void OnComponentAdded(Component* inComponent);
    void OnComponentRemoved(Component* inComponent);

    virtual void OnScaleChanged() = 0;
    
protected:
    physx::PxShape* myShape = nullptr;

private:
    physx::PxRigidStatic* myActor = nullptr;
};
