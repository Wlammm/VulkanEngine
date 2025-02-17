#include "EnginePch.h"
#include "BoxColliderComponent.h"

#include <PxMaterial.h>
#include <PxPhysics.h>
#include <PxRigidStatic.h>

#include "Engine.h"
#include "TransformComponent.h"
#include "Core/Input.h"
#include "Physics/PhysicsSystem.h"
#include "World/World.h"

void BoxColliderComponent::OnCreate()
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    physicsSystem.QueuePhysicsCommand([this, &physicsSystem](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        const glm::vec3 scale = GetTransform()->GetScale();
        
        // Make sure the box is not of 0 size. TODO: This shouldnt be a crash later but for now it is .
        check(myHalfSize.length() * scale.length() > 0);
        
        myShape = inPhysics->createShape(physx::PxBoxGeometry(myHalfSize.x * scale.x, myHalfSize.y * scale.y, myHalfSize.z * scale.z), *physicsSystem.GetDefaultMaterial());
    });
    
    ColliderComponent::OnCreate();
}

void BoxColliderComponent::SetHalfSize(const glm::vec3& inHalfSize)
{
    myHalfSize = inHalfSize;
    OnScaleChanged();
}

void BoxColliderComponent::OnScaleChanged()
{
    const glm::vec3 scale = GetTransform()->GetScale();
    const glm::vec3 newSize = myHalfSize * scale;

    LOG("OnScalechanged");
    // Make sure the box is not of 0 size. TODO: This shouldnt be a crash later but for now it is .
    check(newSize.length() > 0);
    
    Engine::GetEngineSystem<PhysicsSystem>().QueuePhysicsCommand([this, newSize](physx::PxPhysics *,physx::PxScene *)
    {
        physx::PxBoxGeometry geometry = physx::PxBoxGeometry(newSize.x, newSize.y, newSize.z);
        myShape->setGeometry(geometry);
    });
}
