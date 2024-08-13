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

void GameObject::Update()
{
    for(Component* comp : myComponents)
    {
        comp->Update();
    }
}

TransformComponent* GameObject::GetTransform() const
{
    return GetComponent<TransformComponent>();
}