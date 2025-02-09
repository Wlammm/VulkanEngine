#include "EnginePch.h"
#include "Entity.h"

Entity::Entity(const entt::entity inEntity, ECSRegistry* inRegistry)
{
    myEntity = inEntity;
    myRegistry = inRegistry;
}