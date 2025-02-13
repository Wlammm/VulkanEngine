#pragma once
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
    
    void Tick();

    // This tick is called in-between physics updates. It is the only place we're allowed to interact with PhysX directly.
    void TickPhysics();

    template<typename ComponentType, typename... Args>
    ComponentType* AddComponent(Args&&... inArgs)
    {
        ComponentType* component = new ComponentType(std::forward<Args>(inArgs)...);
        component->myGameObject = this;
        myComponents.Add(component);
    
        if(component->DoesComponentTick())
            myTickingComponents.Add(component);

        if(component->DoesComponentImplementOnRenderStateDirty())
            myRenderStateDirtyComponents.Add(component);

        if(component->DoesComponentImplementPhysicsFunctions())
            myPhysicsStateDirtyComponents.Add(component);
        
        component->OnCreate();
        OnComponentAdded(component);
        return component;
    }
    
    template<typename ComponentType>
    ComponentType* GetComponent() const
    {
        static_assert(std::is_base_of<Component, ComponentType>() && "ComponentType is an invalid type. Did you forget to include it?");
        for(Component* comp : myComponents)
        {
            if(ComponentType* castedComponent = dynamic_cast<ComponentType*>(comp))
            {
                return castedComponent;
            }
        }
        return nullptr;
    }

    template<typename ComponentType>
    List<ComponentType*> GetComponents() const
    {
        static_assert(std::is_base_of<Component, ComponentType>() && "ComponentType is an invalid type. Did you forget to include it?");

        List<ComponentType*> returnVal{};
        for(Component* comp : myComponents)
        {
            if(ComponentType* castedComponent = dynamic_cast<ComponentType*>(comp))
            {
                returnVal.Add(castedComponent);
            }
        }
        return returnVal;
    }

    template<typename ComponentType>
    void RemoveComponent()
    {
        ComponentType* component = GetComponent<ComponentType>();

        if(!component)
            return;

        OnPreComponentRemoved(component);
        component->OnDestroy();

        if(component->DoesComponentTick())
            myTickingComponents.Remove(component);

        if(component->DoesComponentImplementOnRenderStateDirty())
            myRenderStateDirtyComponents.Remove(component);

        if(component->DoesComponentImplementPhysicsFunctions())
            myPhysicsStateDirtyComponents.Remove(component);
        
        myComponents.Remove(component);
        delete component;
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
    List<Component*> myComponents{};

    // Optimized lists of components that actually need update every frame.
    List<Component*> myTickingComponents{};
    List<Component*> myRenderStateDirtyComponents{};
    List<Component*> myPhysicsStateDirtyComponents{};

    bool myRenderStateDirty = false;
    bool myPhysicsStateDirty = false;
};