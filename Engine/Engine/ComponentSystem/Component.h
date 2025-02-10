#pragma once
#include "GameObject.h"

class TransformComponent;

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void OnCreate() {}
    virtual void Tick() {}
    // This tick is called in-between physics updates. It is the only place we're allowed to interact with PhysX directly.
    virtual void TickPhysics() {}

    void MarkRenderStateDirty();
    
    // Callback when render state is marked dirty on the gameobject.
    virtual void OnRenderStateDirty() {}

    GameObject* GetGameObject() const;

    TransformComponent* GetTransform() const;

    template<typename ComponentType>
    ComponentType* GetComponent() const
    {
        return myGameObject->GetComponent<ComponentType>();
    }
    
    template<typename ComponentType>
    List<ComponentType*> GetComponents() const
    {
        return myGameObject->GetComponents<ComponentType>();
    }
    
    World* GetWorld() const;

private:
    friend GameObject;
    GameObject* myGameObject;
};