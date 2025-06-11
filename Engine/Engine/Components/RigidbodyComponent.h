#pragma once
#include "Engine/ComponentSystem/Component.h"

namespace physx
{
    class PxRigidDynamic;
}

class ColliderComponent;

enum class ForceMode
{
    Force = 0, 
    Impulse = 1,
    VelocityChange = 2,
    Acceleration = 3
};

// Interacting with most of this classes functions is required to be done in TickPhysics. Those functions has an assert if called outside.
class RigidbodyComponent : public Component
{
    COMP(RigidbodyComponent);
public:
    RigidbodyComponent() = default;
    
    void TickPhysics() override;
    
    void OnCreate() override;
    void OnDestroy() override;

    // This should only be called during TickPhysics.
    void SetVelocity(const glm::vec3& inVelocity);
    
    // This should only be called during TickPhysics.
    glm::vec3 GetVelocity() const;

    // This should only be called during TickPhysics.
    void SetAngularVelocity(const glm::vec3& inAngularVelocity);
    
    // This should only be called during TickPhysics.
    glm::vec3 GetAngularVelocity() const;
    
    // This should only be called during TickPhysics.
    void AddForce(const glm::vec3& inForce, const ForceMode inForceMode);
    
    // This should only be called during TickPhysics.
    void AddTorque(const glm::vec3& inTorque, const ForceMode inForceMode);

    // This should only be called during TickPhysics.
    void SetMass(const float inMass);
    
    // This should only be called during TickPhysics.
    float GetMass() const;

    // This should only be called during TickPhysics.
    void SetDrag(const float inDrag);
    
    // This should only be called during TickPhysics.
    float GetDrag() const;

    // This should only be called during TickPhysics.
    void SetAngularDrag(const float inDrag);
    
    // This should only be called during TickPhysics.
    float GetAngularDrag() const;

    // This should only be called during TickPhysics.
    void UseGravity(const bool inValue);

    // This should only be called during TickPhysics.
    void SetKinematic(const bool inValue);
    
    void AttachCollider(ColliderComponent* inCollider);
    void DetachCollider(ColliderComponent* inCollider);

    void SetRotationConstraint(const bool inX, const bool inY, const bool inZ);
    
private:
    void OnPositionChanged();
    void OnRotationChanged();
    
private:
    physx::PxRigidDynamic* myActor = nullptr;
    int myFramesSinceStartSleep = 0;

    float myMass = 0.0f;
};
