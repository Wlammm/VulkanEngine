#include "EnginePch.h"
#include "SphereColliderComponent.h"

#include <PxPhysics.h>

#include "TransformComponent.h"
#include "Physics/PhysicsSystem.h"
#include "World/World.h"

void SphereColliderComponent::OnCreate()
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    const glm::vec3 scale = GetTransform()->GetScale();
    physicsSystem.QueuePhysicsCommand([this, &physicsSystem, scale](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        myShape = inPhysics->createShape(physx::PxSphereGeometry(myRadius * std::max(scale.x, std::max(scale.y, scale.z))), *physicsSystem.GetDefaultMaterial());
        myShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    });
    
    ColliderComponent::OnCreate();
}

void SphereColliderComponent::SetRadius(const float inRadius)
{
    myRadius = inRadius;
    OnScaleChanged();
}

void SphereColliderComponent::OnScaleChanged()
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();

    const glm::vec3 scale = GetTransform()->GetScale();
    const float newRadius = myRadius * std::max(scale.x, std::max(scale.y, scale.z));
    
    // Make sure the box is not of 0 size. TODO: This shouldnt be a crash later but for now it is.
    check(newRadius > 0);
    physicsSystem.QueuePhysicsCommand([this, newRadius](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        myShape->setGeometry(physx::PxSphereGeometry(newRadius));
    });
}
