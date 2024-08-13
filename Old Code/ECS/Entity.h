#pragma once
#include "ECSRegistry.h"

class Entity
{
public:
    Entity() = default;
    Entity(const entt::entity inEntity, ECSRegistry* inRegistry);

    template<typename ComponentType>
    ComponentType* GetComponent() const
    {
        return myRegistry->GetComponent<ComponentType>(myEntity);
    }
    
private:
    friend ECSRegistry;
    
    ECSRegistry* myRegistry = nullptr;
    entt::entity myEntity{};
};
