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
        for(Component* comp : myRenderStateDirtyComponents)
        {
            comp->OnRenderStateDirty();
        }
        myRenderStateDirty = false;
    }
    
    for(Component* comp : myTickingComponents)
    {
        ZoneScopedN("GameObject::TickComponents");
        comp->Tick();
    }
}

void GameObject::TickPhysics()
{
    ZoneScoped;
    if(myPhysicsStateDirty)
    {
        for(Component* comp : myPhysicsStateDirtyComponents)
        {
            ZoneScopedN("OnPhysicsStateDirty");
            comp->OnPhysicsStateDirty();
        }
        myPhysicsStateDirty = false;
    }
    
    for(Component* comp : myPhysicsStateDirtyComponents)
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
