#pragma once
#include "Delegates/MulticastDelegate.hpp"

class Entity;

class ECSRegistry
{
public:
    ECSRegistry();

    const Entity& CreateEntity();
    const Entity& CreateEntityWithoutTransform();
    
    void DestroyEntity(Entity& inEntity);

    template<typename T, typename... Args>
    T& AddComponent(const Entity& inEntity, Args&&... inArgs)
    {
        return myRegistry.emplace<T>(inEntity.myEntity, std::forward<Args>(inArgs)...);
    }

    template<typename... Types>
    auto IterateComponents()
    {
        auto view = myRegistry.view<Types...>();
        return view.each();
    }

    template<typename... Types>
    size_t NumComponents()
    {
        auto view = myRegistry.view<Types...>();
        return view.size_hint();
    }

    template<typename ComponentType>
    ComponentType* GetComponent(const Entity inEntity) const
    {
        return myRegistry.try_get<ComponentType>(inEntity);
    }

    template<typename ComponentType>
    MulticastDelegate<void(const Entity& inEntity)> OnComponentAdded()
    {
        constexpr std::string typeName = typeid(ComponentType).name();

        if(!myOnComponentAddedDelegates.contains(typeName))
        {
            myOnComponentAddedDelegates.insert({ typeName, MulticastDelegate<void(const Entity& inEntity)>() });
        }
        
        return myOnComponentAddedDelegates[typeName];
    }

    template<typename ComponentType>
    MulticastDelegate<void(const Entity& inEntity)> OnComponentRemoved()
    {
        constexpr std::string typeName = typeid(ComponentType).name();

        if(!myOnComponentRemovedDelegates.contains(typeName))
        {
            myOnComponentRemovedDelegates.insert({ typeName, MulticastDelegate<void(const Entity& inEntity)>() });
        }
        
        return myOnComponentRemovedDelegates[typeName];
    }

private:
    std::unordered_map<std::string, MulticastDelegate<void(const Entity& inEntity)>> myOnComponentAddedDelegates{};
    std::unordered_map<std::string, MulticastDelegate<void(const Entity& inEntity)>> myOnComponentRemovedDelegates{};
    entt::registry myRegistry{};
};