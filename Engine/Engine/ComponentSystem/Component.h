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

    virtual void OnTriggerEnter(GameObject* inOther) {}
    virtual void OnTrigger(GameObject* inOther) {}
    virtual void OnTriggerExit(GameObject* inOther) {}
    
    virtual void OnCollisionEnter(GameObject* inOther) {}
    virtual void OnCollision(GameObject* inOther) {}
    virtual void OnCollisionExit(GameObject* inOther) {}
    
    // This function does not get called unless DoesComponentTick is implemented and returns true.
    virtual void Tick() {}
    
    // This tick is called in-between physics updates. It is the only place we're allowed to interact with PhysX directly.
    // Component needs to have DoesComponentImplementPhysicsFunctions implemented and returning true for this function to be called.
    virtual void TickPhysics() {}

    void MarkRenderStateDirty();
    void MarkPhysicsStateDirty();

    // Callback when render state is marked dirty on the gameobject. Needs to have DoesComponentImplementOnRenderStateDirty() implemeneted and return true for this to take effect.
    virtual void OnRenderStateDirty() {}

    // Called whenever the transform has been tampered with outside of physics. It is safe to interact with physx during this callback as its called inside TickPhysics.
    // Component needs to have DoesComponentImplementPhysicsFunctions implemented and returning true for this function to be called.
    virtual void OnPhysicsStateDirty() {}
    
    GameObject* GetGameObject() const;

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
        return myGameObject->GetComponent<ComponentType>();
    }

    World* GetWorld() const;

private:
    friend GameObject;
    GameObject* myGameObject;
};