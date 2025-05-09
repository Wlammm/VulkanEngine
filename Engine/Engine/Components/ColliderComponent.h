#pragma once
#include "RigidbodyComponent.h"
#include "Engine/ComponentSystem/Component.h"

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

    // An offset applied to the position of the physX actor.
    void SetPhysicsOffset(const glm::vec3& inOffset);
    
protected:
    physx::PxShape* myShape = nullptr;

    glm::vec3 myOffset = {0, 0, 0};
    
    // This will be filled in automatically most of the time. But needed for landscape collider.
    physx::PxRigidStatic* myActor = nullptr;
};
