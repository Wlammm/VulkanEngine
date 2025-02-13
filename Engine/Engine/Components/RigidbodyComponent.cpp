#include "EnginePch.h"
#include "RigidbodyComponent.h"

#include <PxPhysics.h>
#include <PxRigidDynamic.h>
#include <PxScene.h>
#include <extensions/PxRigidBodyExt.h>

#include "ColliderComponent.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "Core/Input.h"
#include "Physics/PhysicsSystem.h"
#include "World/World.h"

RigidbodyComponent::RigidbodyComponent()
{
}

void RigidbodyComponent::TickPhysics()
{
    Component::TickPhysics();

    // No need to update position if its sleeping. This would just cause the GPUScene to keep pushing the same position to the GPU each frame.
    if(myActor->isSleeping())
        return;
    
    physx::PxTransform transform = myActor->getGlobalPose();
    GetTransform()->SetPosition(transform.p);
    GetTransform()->SetRotation(transform.q);
}

void RigidbodyComponent::OnCreate()
{
    Component::OnCreate();
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    physicsSystem.QueuePhysicsCommand([this, &physicsSystem](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        myActor = inPhysics->createRigidDynamic(GetTransform()->AsPxTransform());
        myActor->setGlobalPose(GetTransform()->AsPxTransform());

        myActor->setMaxLinearVelocity(83000.0f);
        myActor->setMaxAngularVelocity(83000.0f);
        myActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
        myActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);

        physicsSystem.GetScene()->addActor(*myActor);
        physx::PxRigidBodyExt::updateMassAndInertia(*myActor, 1.0f);

        // Notify already existing colliders of this components existence.
        // TODO: Create a generalized system for subscribing on gameobject changes like component addition and such.
        List<ColliderComponent*> colliderComponents = GetComponents<ColliderComponent>();
        //for (ColliderComponent* colliderComponent : colliderComponents)
        //{
        //    colliderComponent->OnComponentAdded(this);
        //}
    });
}

void RigidbodyComponent::OnDestroy()
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        inScene->removeActor(*myActor);
    });
}

void RigidbodyComponent::OnPhysicsStateDirty()
{
    myActor->setGlobalPose(GetTransform()->AsPxTransform());
}

void RigidbodyComponent::AttachCollider(ColliderComponent* inCollider)
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this, inCollider](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        myActor->attachShape(*inCollider->GetShape());
        physx::PxRigidBodyExt::updateMassAndInertia(*myActor, 1.0f);
    });
}

void RigidbodyComponent::DetachCollider(ColliderComponent* inCollider)
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this, inCollider](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        myActor->detachShape(*inCollider->GetShape());
        physx::PxRigidBodyExt::updateMassAndInertia(*myActor, 1.0f);
    });
}
