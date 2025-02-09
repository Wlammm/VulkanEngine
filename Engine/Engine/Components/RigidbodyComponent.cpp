#include "EnginePch.h"
#include "RigidbodyComponent.h"

#include <PxPhysics.h>
#include <PxRigidDynamic.h>
#include <PxScene.h>
#include <extensions/PxRigidBodyExt.h>

#include "ColliderComponent.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "Physics/PhysicsSystem.h"
#include "World/World.h"

RigidbodyComponent::RigidbodyComponent()
{
}

void RigidbodyComponent::TickPhysics()
{
    Component::TickPhysics();

    physx::PxTransform transform = myActor->getGlobalPose();
    GetTransform()->SetPosition(transform.p);
    GetTransform()->SetRotation(transform.q);
    LOG("Setting transform to: %s", glm::to_string(GetTransform()->GetPosition()).c_str());
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
        myActor->setMass(1.0f);
        myActor->setLinearDamping(0);

        physicsSystem.GetScene()->addActor(*myActor);
        physx::PxRigidBodyExt::updateMassAndInertia(*myActor, 0.1f);

        // Notify already existing colliders of this components existence.
        // TODO: Create a generalized system for subscribing on gameobject changes like component addition and such.
        List<ColliderComponent*> colliderComponents = GetComponents<ColliderComponent>();
        for (ColliderComponent* colliderComponent : colliderComponents)
        {
         colliderComponent->OnRigidbodyCreated(this);
        }
    });
}

void RigidbodyComponent::AttachCollider(ColliderComponent* inCollider)
{
    myActor->attachShape(*inCollider->GetShape());
}

void RigidbodyComponent::DetachCollider(ColliderComponent* inCollider)
{
    myActor->detachShape(*inCollider->GetShape());
}
