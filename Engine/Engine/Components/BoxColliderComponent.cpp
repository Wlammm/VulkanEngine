#include "EnginePch.h"
#include "BoxColliderComponent.h"

#include <PxMaterial.h>
#include <PxPhysics.h>

#include "Engine.h"
#include "Physics/PhysicsSystem.h"
#include "World/World.h"

BoxColliderComponent::BoxColliderComponent()
{
    
}

BoxColliderComponent::~BoxColliderComponent()
{
    
}

void BoxColliderComponent::OnCreate()
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    physicsSystem.QueuePhysicsCommand([this, &physicsSystem](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        // TODO: Should this be exclusive?
        myShape = inPhysics->createShape(physx::PxBoxGeometry(mySize.x, mySize.y, mySize.z), *physicsSystem.GetDefaultMaterial(), true);
        physx::PxMaterial* val = physicsSystem.GetDefaultMaterial();
        myShape->setMaterials(&val, 1);
        check(mySize.length() > 0);
        myShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    });
    
   

    ColliderComponent::OnCreate();
}
