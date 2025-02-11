#include "EnginePch.h"
#include "SphereColliderComponent.h"

#include <PxPhysics.h>

#include "Physics/PhysicsSystem.h"
#include "World/World.h"

void SphereColliderComponent::OnCreate()
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    physicsSystem.QueuePhysicsCommand([this, &physicsSystem](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        // TODO: Should this be exclusive?
        myShape = inPhysics->createShape(physx::PxSphereGeometry(myRadius), *physicsSystem.GetDefaultMaterial(), true);
        myShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    });
    
    ColliderComponent::OnCreate();
}
