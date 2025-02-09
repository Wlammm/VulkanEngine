#pragma once

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
        component->OnCreate();
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

        myComponents.Remove(component);
    }

    TransformComponent* GetTransform() const;
    void Destroy();

    void MarkRenderStateDirty();

    World* GetWorld() const;
    
private:
    friend ComponentSystem;
    ComponentSystem* myComponentSystem = nullptr;
    List<Component*> myComponents{};

    bool myRenderStateDirty = false;
};