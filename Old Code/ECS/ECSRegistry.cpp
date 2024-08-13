#include "EnginePch.h"
#include "ECSRegistry.h"
#include "Entity.h"
#include "Components/Transform.h"

ECSRegistry::ECSRegistry()
{
    
}

const Entity& ECSRegistry::CreateEntity()
{
    Entity entity = Entity(myRegistry.create(), this);
    AddComponent<Transform>(entity);
    return entity;
}

const Entity& ECSRegistry::CreateEntityWithoutTransform()
{
    return Entity(myRegistry.create(), this);    
}

void ECSRegistry::DestroyEntity(Entity& inEntity)
{
    myRegistry.destroy(inEntity.myEntity);
}