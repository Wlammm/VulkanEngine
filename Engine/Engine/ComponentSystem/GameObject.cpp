#include "EnginePch.h"
#include "GameObject.h"

#include "Component.h"
#include "ComponentSystem.h"
#include "Components/TransformComponent.h"

GameObject::~GameObject()
{
    // Remove all components.
    for(Component* comp : GetComponents())
    {
        comp->OnDestroy();
    }
    
    const List<IComponentArray*>& arrays = myComponentSystem->GetAllComponentArrays();
    for(IComponentArray* array : arrays)
    {
        array->TryRemoveComponentForGameObject(this);
    }
}

bool GameObject::IsRenderStateDirty() const
{
    return myRenderStateDirty;
}

bool GameObject::IsPhysicsStateDirty() const
{
    return myPhysicsStateDirty;
}

void GameObject::ResetRenderStateDirtyFlag()
{
    myRenderStateDirty = false;
}

void GameObject::ResetPhysicsStateDirtyFlag()
{
    myPhysicsStateDirty = false;
}

List<Component*> GameObject::GetComponents() const
{
    List<Component*> components;

    for(IComponentArray* array : myComponentSystem->GetAllComponentArrays())
    {
        if(Component* comp = array->TryGetComponentForGameObject(this))
            components.Add(comp);
    }
    return components;
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