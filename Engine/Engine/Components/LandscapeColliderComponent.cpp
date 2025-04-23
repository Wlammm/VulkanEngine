#include "EnginePch.h"
#include "LandscapeColliderComponent.h"

#include <PxPhysics.h>

#include "TransformComponent.h"
#include "Physics/PhysicsSystem.h"
#include "World/World.h"

LandscapeColliderComponent::LandscapeColliderComponent()
{
}

void LandscapeColliderComponent::OnCreate()
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    physicsSystem.QueuePhysicsCommand([this, &physicsSystem](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        const glm::vec3 scale = GetTransform()->GetScale();

        // Create heightfield collider here.
    });
    
    ColliderComponent::OnCreate();
}
