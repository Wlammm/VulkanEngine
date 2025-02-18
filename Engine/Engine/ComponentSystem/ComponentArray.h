#pragma once
#include <unordered_map>

#include "Engine/Containers/SegmentedList.h"

class GameObject;

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void Tick() = 0;
    virtual void TickPhysics() = 0;

    virtual void TryRemoveComponentForGameObject(const GameObject* inObject) = 0;
    virtual class Component* TryGetComponentForGameObject(const GameObject* inObject) = 0;
};

/*
 * 
 */
template<typename ComponentType>
class ComponentArray : public IComponentArray
{
public:
    // The amount of components each segment consists of.
    inline static constexpr int ComponentsPerSegment = 128;
    
    ComponentType* GetFirstComponentOrNull()
    {
        if(myComponents.IsEmpty())
            return nullptr;

        return &myComponents.First();
    }
    
    void Tick() override
    {
        if constexpr (ImplementsOnRenderStateDirty())
        {
            for(ComponentType& component : myComponents)
            {
                if(component.GetGameObject()->IsRenderStateDirty())
                {
                    component.ComponentType::OnRenderStateDirty();
                    component.GetGameObject()->ResetRenderStateDirtyFlag();
                }
            }
        }

        if constexpr (ImplementsTick())
        {
            for(ComponentType& component : myComponents)
            {
                component.ComponentType::Tick();
            }
        }
    }

    template<typename A = Component, typename B = ComponentType>
    static consteval bool ImplementsTick()
    {
        return !std::is_same_v<decltype(&A::Tick), decltype(&B::Tick)>;
    }

    template<typename A = Component, typename B = ComponentType>
    static consteval bool ImplementsOnRenderStateDirty()
    {
        return !std::is_same_v<decltype(&A::OnRenderStateDirty), decltype(&B::OnRenderStateDirty)>;
    }

    template<typename A = Component, typename B = ComponentType>
    static consteval bool ImplementsTickPhysics()
    {
        return !std::is_same_v<decltype(&A::TickPhysics), decltype(&B::TickPhysics)>;
    }

    template<typename A = Component, typename B = ComponentType>
    static consteval bool ImplementsOnPhysicsStateDirty()
    {
        return !std::is_same_v<decltype(&A::OnPhysicsStateDirty), decltype(&B::OnPhysicsStateDirty)>;
    }

    void TickPhysics() override
    {
        if constexpr (ImplementsOnPhysicsStateDirty())
        {
            for(ComponentType& component : myComponents)
            {
                if(component.GetGameObject()->IsPhysicsStateDirty())
                {
                    component.ComponentType::OnPhysicsStateDirty();
                    component.GetGameObject()->ResetPhysicsStateDirtyFlag();
                }
            }
        }
        
        if constexpr (ImplementsTickPhysics())
        {
            for(ComponentType& component : myComponents)
            {
                component.ComponentType::TickPhysics();
            }
        }
    }

    template<typename... Args>
    ComponentType* AddComponentForGameObject(GameObject* inGameObject, Args&&... inArgs)
    {
        check(myGameObjectToComponentIndex.find(inGameObject) == myGameObjectToComponentIndex.end() && "Object already have one of these components on it. Only one of each type is allowed.");
        
        uint index;
        ComponentType& component = myComponents.EmplaceAndGetIndex(index, std::forward<Args>(inArgs)...);
        myGameObjectToComponentIndex.insert({ inGameObject, index });
        
        return &component;
    }

    void TryRemoveComponentForGameObject(const GameObject* inObject) override
    {
        if(myGameObjectToComponentIndex.find(inObject) == myGameObjectToComponentIndex.end())
            return;

        RemoveComponentForGameObject(inObject);
    }

    class Component* TryGetComponentForGameObject(const GameObject* inObject) override
    {
        const auto iter = myGameObjectToComponentIndex.find(inObject);
        if(iter == myGameObjectToComponentIndex.end())
            return nullptr;
        
        return &myComponents[iter->second];
    }

    void RemoveComponentForGameObject(const GameObject* inGameObject)
    {
        const auto iter = myGameObjectToComponentIndex.find(inGameObject);

        // The gameobject should keep track of what component types each gameobject has.
        check(iter != myGameObjectToComponentIndex.end());
        
        myComponents.RemoveIndex(iter->second);
        myGameObjectToComponentIndex.erase(iter);
    }

    bool HasComponentForGameObject(const GameObject* inObject) const
    {
        return myGameObjectToComponentIndex.find(inObject) != myGameObjectToComponentIndex.end();
    }
    
    ComponentType* GetComponentForGameObject(const GameObject* inObject)
    {
        auto iter = myGameObjectToComponentIndex.find(inObject);
        if(iter == myGameObjectToComponentIndex.end())
            return nullptr;

        return &myComponents[iter->second];
    }

    const SegmentedList<ComponentType, ComponentsPerSegment>& GetAllComponents() const
    {
        return myComponents;
    }

private:
    SegmentedList<ComponentType, ComponentsPerSegment> myComponents;
    
    // TODO: Investigate performance impact of this vs an ordered map.
    std::unordered_map<const GameObject*, uint> myGameObjectToComponentIndex;
};
