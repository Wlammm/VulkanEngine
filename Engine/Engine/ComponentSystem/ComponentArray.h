#pragma once
#include <unordered_map>

#include "Engine/Containers/SegmentedList.h"
#include "plf/Colony.hpp"

class GameObject;

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void Tick() = 0;
    virtual void TickPhysics() = 0;

    virtual void TryRemoveComponentForGameObject(const GameObject* inObject) = 0;
    virtual class Component* TryGetComponentForGameObject(const GameObject* inObject) = 0;
    
    virtual void OnTriggerEnterForGameObject(GameObject* inObject, GameObject* inOther) = 0;
    virtual void OnTriggerForGameObject(GameObject* inObject, GameObject* inOther) = 0;
    virtual void OnTriggerExitForGameObject(GameObject* inObject, GameObject* inOther) = 0;
    
    virtual void OnCollisionEnterForGameObject(GameObject* inObject, GameObject* inOther) = 0;
    virtual void OnCollisionForGameObject(GameObject* inObject, GameObject* inOther) = 0;
    virtual void OnCollisionExitForGameObject(GameObject* inObject, GameObject* inOther) = 0;
};

#define IMPLEMENTS_METHOD(methodName)\
template<typename A = Component, typename B = ComponentType> \
static consteval bool Implements##methodName() \
{ \
    return !std::is_same_v<decltype(&A::methodName), decltype(&B::methodName)>; \
}

/*
 * 
 */
template<typename ComponentType>
class ComponentArray : public IComponentArray
{
public:
    // The amount of components each segment consists of.
    inline static constexpr int ComponentsPerSegment = 128;

    IMPLEMENTS_METHOD(Tick);
    IMPLEMENTS_METHOD(TickPhysics);
    IMPLEMENTS_METHOD(OnRenderStateDirty);
    
    IMPLEMENTS_METHOD(OnTriggerEnter);
    IMPLEMENTS_METHOD(OnTrigger);
    IMPLEMENTS_METHOD(OnTriggerExit);
    
    IMPLEMENTS_METHOD(OnCollisionEnter);
    IMPLEMENTS_METHOD(OnCollision);
    IMPLEMENTS_METHOD(OnCollisionExit);
    
    ComponentType* GetFirstComponentOrNull()
    {
        if(myComponents.empty())
            return nullptr;

        return &(*myComponents.begin());
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

    void TickPhysics() override
    {
        if constexpr (ImplementsTickPhysics())
        {
            for(ComponentType& component : myComponents)
            {
                component.ComponentType::TickPhysics();
            }
        }
    }

    void OnTriggerEnterForGameObject(GameObject* inObject, GameObject* inOther) override
    {
        if constexpr (!ImplementsOnTriggerEnter())
            return;

        Component* comp = TryGetComponentForGameObject(inObject);
        if(!comp)
            return;
        
        comp->OnTriggerEnter(inOther);
    }

    void OnTriggerForGameObject(GameObject* inObject, GameObject* inOther) override
    {
        if constexpr (!ImplementsOnTrigger())
            return;

        Component* comp = TryGetComponentForGameObject(inObject);
        if(!comp)
            return;
        
        comp->OnTrigger(inOther);
    }

    void OnTriggerExitForGameObject(GameObject* inObject, GameObject* inOther) override
    {
        if constexpr (!ImplementsOnTriggerExit())
            return;

        Component* comp = TryGetComponentForGameObject(inObject);
        if(!comp)
            return;
        
        comp->OnTriggerExit(inOther);
    }

    void OnCollisionEnterForGameObject(GameObject* inObject, GameObject* inOther) override
    {
        if constexpr (!ImplementsOnCollisionEnter())
            return;

        Component* comp = TryGetComponentForGameObject(inObject);
        if(!comp)
            return;
        
        comp->OnCollisionEnter(inOther);
    }

    void OnCollisionForGameObject(GameObject* inObject, GameObject* inOther) override
    {
        if constexpr (!ImplementsOnCollision())
            return;

        Component* comp = TryGetComponentForGameObject(inObject);
        if(!comp)
            return;
        
        comp->OnCollision(inOther);
    }

    void OnCollisionExitForGameObject(GameObject* inObject, GameObject* inOther) override
    {
        if constexpr (!ImplementsOnCollisionExit())
            return;

        Component* comp = TryGetComponentForGameObject(inObject);
        if(!comp)
            return;
        
        comp->OnCollisionExit(inOther);
    }

    template<typename... Args>
    ComponentType* AddComponentForGameObject(GameObject* inGameObject, Args&&... inArgs)
    {
        check(myGameObjectToComponentIndex.find(inGameObject) == myGameObjectToComponentIndex.end() && "Object already have one of these components on it. Only one of each type is allowed.");

        auto iterator = myComponents.emplace(std::forward<Args>(inArgs)...);
        myGameObjectToComponentIndex.insert({ inGameObject, iterator });
        
        return &(*iterator);
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

        return &(*iter->second);
    }

    void RemoveComponentForGameObject(const GameObject* inGameObject)
    {
        const auto iter = myGameObjectToComponentIndex.find(inGameObject);

        // The gameobject should keep track of what component types each gameobject has.
        check(iter != myGameObjectToComponentIndex.end());
        
        myComponents.erase(iter->second);
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

        return &(*iter->second);
    }

    const plf::colony<ComponentType>& GetAllComponents() const
    {
        return myComponents;
    }

private:
    plf::colony<ComponentType> myComponents{};
    
    // TODO: Investigate performance impact of this vs an ordered map.
    std::unordered_map<const GameObject*, typename plf::colony<ComponentType>::iterator> myGameObjectToComponentIndex;
};
