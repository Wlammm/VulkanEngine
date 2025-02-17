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

void ColliderComponent::OnCreate()
{
    Component::OnCreate();

    GetTransform()->OnScaleChanged.Bind(&ColliderComponent::OnScaleChanged, this);

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

    GetGameObject()->OnComponentAdded.Bind(&ColliderComponent::OnComponentAdded, this);
    GetGameObject()->OnComponentRemoved.Bind(&ColliderComponent::OnComponentRemoved, this);
}

void ColliderComponent::OnDestroy()
{
    check(false && "Need to implement destruction of shapes and actors here.");
    GetTransform()->OnScaleChanged.UnBind(&ColliderComponent::OnScaleChanged, this);
    
    GetGameObject()->OnComponentAdded.UnBind(&ColliderComponent::OnComponentAdded, this);
    GetGameObject()->OnComponentRemoved.UnBind(&ColliderComponent::OnComponentRemoved, this);
}

void ColliderComponent::OnPhysicsStateDirty()
{
    Component::OnPhysicsStateDirty();

    if(!myActor)
        return;

    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        myActor->setGlobalPose(GetTransform()->AsPxTransform());
    });
}

physx::PxShape* ColliderComponent::GetShape() const
{
    return myShape;
}

void ColliderComponent::SetIsTrigger(const bool inIsTrigger) const
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this, inIsTrigger](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        if(myShape)
        {
            myShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
            myShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
            myShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !inIsTrigger);
            myShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, inIsTrigger);
        }
    });
}

void ColliderComponent::OnComponentAdded(Component* inComponent)
{
    if(!inComponent->IsA<RigidbodyComponent>())
        return;

    RigidbodyComponent* rigidbody = static_cast<RigidbodyComponent*>(inComponent);
    
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this, rigidbody](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        if(myActor)
        {
            myActor->detachShape(*myShape);
            myActor->release();
            myActor = nullptr;
        }
    });

    rigidbody->AttachCollider(this);
}

void ColliderComponent::OnComponentRemoved(Component* inComponent)
{
    if(!inComponent->IsA<RigidbodyComponent>())
        return;

    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        myActor = inPhysics->createRigidStatic(GetTransform()->AsPxTransform());
        myActor->attachShape(*myShape);
        inScene->addActor(*myActor);
    });
}
