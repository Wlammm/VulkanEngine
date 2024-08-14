#include "EnginePch.h"
#include "GameObject.h"

#include "Component.h"
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