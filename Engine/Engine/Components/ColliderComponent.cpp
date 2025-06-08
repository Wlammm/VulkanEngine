#include "EnginePch.h"
#include "ColliderComponent.h"

#include <PxPhysics.h>
#include <PxRigidStatic.h>
#include <PxScene.h>

#include "Engine/Engine.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "Engine/Physics/PhysicsSystem.h"
#include "Engine/World/World.h"

void ColliderComponent::OnCreate()
{
    Component::OnCreate();

    GetTransform()->OnPositionChanged.Bind(&ColliderComponent::OnPhysicsStateDirty, this);
    GetTransform()->OnRotationChanged.Bind(&ColliderComponent::OnPhysicsStateDirty, this);
    GetTransform()->OnScaleChanged.Bind(&ColliderComponent::OnScaleChanged, this);

    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    RigidbodyComponent* rigidbody = GetComponent<RigidbodyComponent>();
    physicsSystem.QueuePhysicsCommand([this, rigidbody](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        check(myShape && "Shape must be initialized before calling this function.");
        check(myShape->getReferenceCount() == 1 && "Reference count should always be one here until we add support for non exclusive shapes.");
        ApplyLocalPose();
        
        if (rigidbody)
        {
            rigidbody->AttachCollider(this);
            return;
        }
        auto transform = GetTransform();
        myActor = inPhysics->createRigidStatic(GetTransform()->AsPxTransform());
        myActor->userData = GetGameObject();
        
        myActor->attachShape(*myShape);
        
        // Call release on shape so it gets destroyed whenever the actor is removed from the scene.
        myShape->release();

        inScene->addActor(*myActor);
    });

    GetGameObject()->OnComponentAdded.Bind(&ColliderComponent::OnComponentAdded, this);
    GetGameObject()->OnComponentRemoved.Bind(&ColliderComponent::OnComponentRemoved, this);
}

void ColliderComponent::OnDestroy()
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        if(myShape)
            myShape->release();
        
        if(myActor)
        {
            inScene->removeActor(*myActor);
            myActor->release();
        }
    });

    GetTransform()->OnPositionChanged.UnBind(&ColliderComponent::OnPhysicsStateDirty, this);
    GetTransform()->OnRotationChanged.UnBind(&ColliderComponent::OnPhysicsStateDirty, this);
    GetTransform()->OnScaleChanged.UnBind(&ColliderComponent::OnScaleChanged, this);
    
    GetGameObject()->OnComponentAdded.UnBind(&ColliderComponent::OnComponentAdded, this);
    GetGameObject()->OnComponentRemoved.UnBind(&ColliderComponent::OnComponentRemoved, this);
}

void ColliderComponent::OnPhysicsStateDirty()
{
    check(!PhysicsSystem::IsSimulatingPhysics);
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
            // Add a reference here before we detach the shape and release in rigidbody. Otherwise it will be garbage collected before we can add it to our new actor.
            myShape->acquireReference();
            
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

void ColliderComponent::SetLocalShapePosition(const glm::vec3& inOffset)
{
    myLocalShapePosition = inOffset;
    QueueLocalPoseUpdate();
}

void ColliderComponent::SetLocalShapeRotation(const glm::vec3& inRotation)
{
    myLocalShapeRotation = inRotation;
    QueueLocalPoseUpdate();
}

void ColliderComponent::QueueLocalPoseUpdate() const
{
    if (!myShape)
        return;
    
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        ApplyLocalPose();
    });
}

void ColliderComponent::ApplyLocalPose() const
{
    glm::vec3 pos = myLocalShapePosition;
    glm::quat quat = glm::quat(glm::radians(myLocalShapeRotation));
	    
    physx::PxTransform transform;
    transform.p = {pos.x, pos.y, pos.z};
    transform.q.x = quat.x;
    transform.q.y = quat.y;
    transform.q.z = quat.z;
    transform.q.w = quat.w;

    myShape->setLocalPose(transform);
}
