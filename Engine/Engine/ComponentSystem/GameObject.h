#pragma once
#include "ComponentArray.h"
#include "ComponentSystem.h"
#include "Engine/Delegates/MulticastDelegate.hpp"

class World;
class TransformComponent;
class ComponentSystem;
class Component;

class GameObject
{
public:
    GameObject() = default;
    ~GameObject();
    
    bool IsRenderStateDirty() const;
    bool IsPhysicsStateDirty() const;

    void ResetRenderStateDirtyFlag();
    void ResetPhysicsStateDirtyFlag();

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

    bool myRenderStateDirty = false;
    bool myPhysicsStateDirty = false;
};