#pragma once

class World;
class TransformComponent;
class Actor;

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void OnCreate() {}
    virtual void OnDestroy() {}

    virtual void EditorTick() {}
    
    // This function does not get called unless DoesComponentTick is implemented and returns true.
    virtual void Tick() {}
    
    // This tick is called in-between physics updates. It is the only place we're allowed to interact with PhysX directly.
    // Component needs to have DoesComponentImplementPhysicsFunctions implemented and returning true for this function to be called.
    virtual void TickPhysics() {}

    void MarkRenderStateDirty();

    // Callback when render state is marked dirty on the gameobject. Needs to have DoesComponentImplementOnRenderStateDirty() implemeneted and return true for this to take effect.
    virtual void OnRenderStateDirty() {}

    virtual void OnTriggerEnter(Actor* inOther) {}
    virtual void OnTrigger(Actor* inOther) {}
    virtual void OnTriggerExit(Actor* inOther) {}

    virtual void OnCollisionEnter(Actor* inOther) {}
    virtual void OnCollision(Actor* inOther) {}
    virtual void OnCollisionExit(Actor* inOther) {}
    
    
    template<typename T>
    bool IsA() const
    {
        return dynamic_cast<const T*>(this) != nullptr;        
    }
    
    TransformComponent& GetTransform() const;

    Actor* GetActor() const;
    World* GetWorld() const;

private:
    friend class Actor;
    Actor* myActor = nullptr;
};