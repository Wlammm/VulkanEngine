#pragma once

class TransformComponent;
class GameObject;

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void Start() {}
    virtual void Tick() {}

    void MarkRenderStateDirty();
    
    // Callback when render state is marked dirty on the gameobject.
    virtual void OnRenderStateDirty() {}

    GameObject* GetGameObject() const;

    TransformComponent* GetTransform() const;

private:
    friend GameObject;
    GameObject* myGameObject;
};