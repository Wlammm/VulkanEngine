#pragma once

class TransformComponent;
class ComponentSystem;
class Component;

class GameObject
{
public:
    GameObject() = default;
    ~GameObject();
    
    void Update();

    template<typename ComponentType, typename... Args>
    ComponentType* AddComponent(Args&&... inArgs)
    {
        ComponentType* component = new ComponentType(std::forward<Args>(inArgs)...);
        component->myGameObject = this;
        component->Start();
        myComponents.Add(component);
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
    void RemoveComponent()
    {
        ComponentType* component = GetComponent<ComponentType>();

        if(!component)
            return;

        myComponents.Remove(component);
    }

    TransformComponent* GetTransform() const;

private:
    friend ComponentSystem;
    ComponentSystem* myComponentSystem = nullptr;
    List<Component*> myComponents{};
};