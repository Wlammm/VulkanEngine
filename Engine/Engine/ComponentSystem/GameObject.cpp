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
    if(myRenderStateDirty)
    {
        for(Component* comp : myComponents)
        {
            comp->OnRenderStateDirty();
        }
        myRenderStateDirty = false;
    }
    
    for(Component* comp : myComponents)
    {
        comp->Tick();
    }
}

void GameObject::TickPhysics()
{
    if(myPhysicsStateDirty)
    {
        for(Component* comp : myComponents)
        {
            comp->OnPhysicsStateDirty();
        }
        myPhysicsStateDirty = false;
    }
    
    for(Component* comp : myComponents)
    {
        comp->TickPhysics();
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
    myRenderStateDirty = true;
}

void GameObject::MarkPhysicsStateDirty()
{
    myPhysicsStateDirty = true;
}

World* GameObject::GetWorld() const
{
    return myComponentSystem->GetWorld();
}
