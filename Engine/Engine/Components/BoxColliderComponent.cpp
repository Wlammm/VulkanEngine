#include "EnginePch.h"
#include "BoxColliderComponent.h"

#include <PxMaterial.h>
#include <PxPhysics.h>

#include "Engine.h"
#include "TransformComponent.h"
#include "Core/Input.h"
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
        const glm::vec3 scale = GetTransform()->GetScale();
        

        // Make sure the box is not of 0 size. TODO: This shouldnt be a crash later but for now it is .
        check(myHalfSize.length() * scale.length() > 0);
        
        // TODO: Should this be exclusive?
        myShape = inPhysics->createShape(physx::PxBoxGeometry(myHalfSize.x * scale.x, myHalfSize.y * scale.y, myHalfSize.z * scale.z), *physicsSystem.GetDefaultMaterial(), true);
        physx::PxMaterial* val = physicsSystem.GetDefaultMaterial();
        myShape->setMaterials(&val, 1);
        myShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    });
    
    ColliderComponent::OnCreate();
}

void BoxColliderComponent::Tick()
{
    ColliderComponent::Tick();

    if(Input::IsKeyPressed(KeyCode::K))
        GetTransform()->Move(0, -10, 0);
}
