#include "EnginePch.h"
#include "RigidbodyComponent.h"

#include <PxPhysics.h>
#include <PxRigidDynamic.h>
#include <PxScene.h>
#include <extensions/PxRigidBodyExt.h>

#include "ColliderComponent.h"
#include "Engine/Engine.h"
#include "TransformComponent.h"
#include "Engine/Core/Input.h"
#include "Engine/Physics/PhysicsSystem.h"
#include "Engine/World/World.h"

RigidbodyComponent::RigidbodyComponent()
{
}

void RigidbodyComponent::TickPhysics()
{
    ZoneScoped;
    Component::TickPhysics();

    // Update the position one extra time before going to sleep.
    if(myActor->isSleeping())
    {
        myFramesSinceStartSleep++;

        if(myFramesSinceStartSleep > 2)
            return;
    }
    myFramesSinceStartSleep = 0;

    if (GetGameObject()->IsPhysicsStateDirty())
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
        myActor->userData = GetGameObject();
        
        myActor->setGlobalPose(GetTransform()->AsPxTransform());

        myActor->setMaxLinearVelocity(83000.0f);
        myActor->setMaxAngularVelocity(83000.0f);
        myActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
        myActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);

        physicsSystem.GetScene()->addActor(*myActor);
        physx::PxRigidBodyExt::updateMassAndInertia(*myActor, 1.0f);

        // Notify already existing colliders of this components existence.
        // TODO: Create a generalized system for subscribing on gameobject changes like component addition and such.
        //List<ColliderComponent*> colliderComponents = GetComponents<ColliderComponent>();
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

void RigidbodyComponent::SetVelocity(const glm::vec3& inVelocity)
{
    check(!PhysicsSystem::IsSimulatingPhysics && "This should only be called from TickPhysics");

    if(!myActor)
        return;
    
    myActor->setLinearVelocity(physx::PxVec3(inVelocity.x, inVelocity.y, inVelocity.z));
}

glm::vec3 RigidbodyComponent::GetVelocity() const
{
    check(!PhysicsSystem::IsSimulatingPhysics && "This should only be called from TickPhysics");

    physx::PxVec3 velocity = myActor->getLinearVelocity();
    return glm::vec3(velocity.x, velocity.y, velocity.z);
}

void RigidbodyComponent::AttachCollider(ColliderComponent* inCollider)
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this, inCollider](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        myActor->attachShape(*inCollider->GetShape());
        
        // Remove the reference temporarily added in ColliderComponent::OnComponentAdded. 
        inCollider->GetShape()->release();

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

void RigidbodyComponent::SetRotationConstraint(const bool inX, const bool inY, const bool inZ)
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this, inX, inY, inZ](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        if (!myActor)
            return;
        
        myActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, inX);
        myActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, inY);
        myActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, inZ);
    });
}
