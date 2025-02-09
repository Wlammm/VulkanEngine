#include "EnginePch.h"
#include "GameObject.h"

#include "Component.h"
#include "ComponentSystem.h"
#include "Components/TransformComponent.h"

GameObject::~GameObject()
{
    for(Component* comp : myComponents)
    {
        del(comp);
    }
    myComponents.Clear();
}

void GameObject::Tick()
{
    for(Component* comp : myComponents)
    {
        comp->Tick();
    }
}

TransformComponent* GameObject::GetTransform() const
{
    return GetComponent<TransformComponent>();
}

void GameObject::Destroy()
{
    myComponentSystem->DestroyGameObject(this);
}

void GameObject::MarkRenderStateDirty()
{
    // TODO: This should probably be queued and only updated once per frame. No need to update multiple times a frame.
    for(Component* comp : myComponents)
    {
        comp->OnRenderStateDirty();
    }
}
