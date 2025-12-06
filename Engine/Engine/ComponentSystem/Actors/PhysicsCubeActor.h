#pragma once
#include "Engine/Components/BoxColliderComponent.h"
#include "Engine/Components/StaticMeshComponent.h"
#include "Engine/ComponentSystem/Actor.h"

class PhysicsCubeActor : public Actor
{
public:
    PhysicsCubeActor();

private:
    META(SerializeField)
    StaticMeshComponent myMesh;
    
    META(SerializeField)
    BoxColliderComponent myCollider;
    
    META(SerializeField)
    RigidbodyComponent myRigidbody;
};
