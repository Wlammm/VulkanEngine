#include "EnginePch.h"
#include "CapsuleColliderComponent.h"

#include <PxPhysics.h>

#include "TransformComponent.h"
#include "Physics/PhysicsSystem.h"
#include "Utils/Debug.h"
#include "World/World.h"

void CapsuleColliderComponent::OnCreate()
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    physicsSystem.QueuePhysicsCommand([this, &physicsSystem](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        myShape = inPhysics->createShape(physx::PxCapsuleGeometry(myRadius, myHeight), *physicsSystem.GetDefaultMaterial());
        myShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    });

    ColliderComponent::OnCreate();
}

void CapsuleColliderComponent::SetRadius(const float inRadius)
{
    myRadius = inRadius;
    OnScaleChanged();
}

void CapsuleColliderComponent::SetHeight(const float inHeight)
{
    myHeight = inHeight;
    OnScaleChanged();
}

void CapsuleColliderComponent::OnScaleChanged()
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    physicsSystem.QueuePhysicsCommand([this](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        myShape->setGeometry(physx::PxCapsuleGeometry(myRadius,myHeight));
    });
}