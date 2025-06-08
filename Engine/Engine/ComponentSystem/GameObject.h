#pragma once
#include "ComponentArray.h"
#include "ComponentSystem.h"
#include "Engine/Delegates/MulticastDelegate.hpp"

class World;
class TransformComponent;
class ComponentSystem;
class Component;

#ifndef DEBUG_GAMEOBJECT_NAMES
#define DEBUG_GAMEOBJECT_NAMES DEBUG
#endif

using TagMask = uint32_t;

class GameObject
{
public:
    GameObject() = default;
    ~GameObject();
    
    bool IsRenderStateDirty() const;

    void ResetRenderStateDirtyFlag();

    List<Component*> GetComponents() const;
    
    // TODO: Need to find new way of referencing components. We cant reference them via pointers as they move around in memory. Either that or implement some type of segmented list in ComponentArray.
    template<typename ComponentType, typename... Args>
    ComponentType* AddComponent(Args&&... inArgs)
    {
        ComponentArray<ComponentType>& componentArray = myComponentSystem->GetComponentArrayForType<ComponentType>();
        ComponentType* component = componentArray.AddComponentForGameObject(this, std::forward<Args>(inArgs)...);
        component->myGameObject = this;

        component->OnCreate();
        OnComponentAdded(component);
        return component;
    }
    
    template<typename ComponentType>
    ComponentType* GetComponent() const
    {
        static_assert(std::is_base_of<Component, ComponentType>() && "ComponentType is an invalid type. Did you forget to include it?");
        ComponentArray<ComponentType>& componentArray = myComponentSystem->GetComponentArrayForType<ComponentType>();

        // TODO: This is doing double work. Maybe have the gameobject know if the component exists on it or not?
        if(!componentArray.HasComponentForGameObject(this))
            return nullptr;
        
        return componentArray.GetComponentForGameObject(this);
    }

    template<typename ComponentType>
    void RemoveComponent()
    {
        static_assert(std::is_base_of<Component, ComponentType>() && "ComponentType is an invalid type. Did you forget to include it?");
        LOG("Remove Component called.");
        ComponentArray<ComponentType>& componentArray = myComponentSystem->GetComponentArrayForType<ComponentType>();

        ComponentType* component = GetComponent<ComponentType>();
        OnComponentRemoved(component);
        component->OnDestroy();
        
        componentArray.RemoveComponentForGameObject(this);
    }

    TransformComponent* GetTransform() const;
    void Destroy();

    TagMask GetTags() const;
    void AddTags(TagMask inTags);
    void RemoveTags(TagMask inTags);
    void ClearTags();
    bool HasAnyTag(TagMask inTags) const;
    bool HasAllTags(TagMask inTags) const;
    
    void MarkRenderStateDirty();
    void MarkPhysicsStateDirty();

    World* GetWorld() const;

    // Called after a component was added. Right after Component::OnCreate
    MulticastDelegate<void(Component*)> OnComponentAdded;
    
    // Called right before a component is deleted.
    MulticastDelegate<void(Component*)> OnComponentRemoved;

private:
    friend ComponentSystem;
    ComponentSystem* myComponentSystem = nullptr;
    TransformComponent* myTransform = nullptr;

    bool myRenderStateDirty = false;
    bool myPhysicsStateDirty = false;

    TagMask myTags;

#if DEBUG_GAMEOBJECT_NAMES
    std::string myName = "Unnamed GameObject";
#endif
};