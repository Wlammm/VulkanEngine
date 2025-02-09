#include "EnginePch.h"
#include "ColliderComponent.h"

#include <PxPhysics.h>
#include <PxRigidStatic.h>
#include <PxScene.h>

#include "Engine.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "Physics/PhysicsSystem.h"
#include "World/World.h"

ColliderComponent::ColliderComponent()
{
}

void ColliderComponent::OnCreate()
{
    Component::OnCreate();
    
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    RigidbodyComponent* rigidbody = GetComponent<RigidbodyComponent>();
    physicsSystem.QueuePhysicsCommand([this, rigidbody](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        check(myShape && "Shape must be initialized before calling this function.");
        
        if (rigidbody)
        {
            rigidbody->AttachCollider(this);
            return;
        }
        myActor = inPhysics->createRigidStatic(GetTransform()->AsPxTransform());
        myActor->attachShape(*myShape);
        inScene->addActor(*myActor);
    });
}

physx::PxShape* ColliderComponent::GetShape() const
{
    return myShape;
}

void ColliderComponent::OnRigidbodyCreated(RigidbodyComponent* inRigidbody)
{
    if(myActor)
    {
        myActor->detachShape(*myShape);
        myActor->release();
        myActor = nullptr;
    }

    inRigidbody->AttachCollider(this);
}