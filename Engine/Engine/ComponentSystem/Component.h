#pragma once

class TransformComponent;
class GameObject;

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void Start() {}
    virtual void Update() {}

    GameObject* GetGameObject() const;

    TransformComponent& GetTransform() const;

private:
    friend GameObject;
    GameObject* myGameObject;
};
