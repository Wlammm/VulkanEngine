#pragma once
#include "GameObject.h"

class TransformComponent;

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void OnCreate() {}
    virtual void OnDestroy() {}

    virtual void OnTriggerEnter(const GameObject& inOther) {}
    virtual void OnTrigger(const GameObject& inOther) {}
    virtual void OnTriggerExit(const GameObject& inOther) {}
    
    virtual void OnCollisionEnter(const GameObject& inOther) {}
    virtual void OnCollision(const GameObject& inOther) {}
    virtual void OnCollisionExit(const GameObject& inOther) {}
    
    // This function does not get called unless DoesComponentTick is implemented and returns true.
    virtual void Tick() {}
    
    // This tick is called in-between physics updates. It is the only place we're allowed to interact with PhysX directly.
    // Component needs to have DoesComponentImplementPhysicsFunctions implemented and returning true for this function to be called.
    virtual void TickPhysics() {}

    void MarkRenderStateDirty();

    // Callback when render state is marked dirty on the gameobject. Needs to have DoesComponentImplementOnRenderStateDirty() implemeneted and return true for this to take effect.
    virtual void OnRenderStateDirty() {}

    const GameObject& GetGameObject() const;

    TransformComponent* GetTransform() const;

    template<typename ComponentType>
    bool IsA()
    {
        static_assert(std::is_base_of<Component, ComponentType>::value && "ComponentType must derive from Component");
        
        // TODO: Replace this with faster check whenever we have the posibility for this.
        return dynamic_cast<ComponentType*>(this) != nullptr;
    }
    
    template<typename ComponentType>
    ComponentType* GetComponent() const
    {
        return myGameObject.GetComponent<ComponentType>();
    }

    World* GetWorld() const;

private:
    friend GameObject;
    GameObject myGameObject;
};