#include "EnginePch.h"
#include "GameObject.h"

#include "Component.h"
#include "ComponentSystem.h"
#include "Engine/Components/TransformComponent.h"

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

void GameObject::ResetRenderStateDirtyFlag()
{
    myRenderStateDirty = false;
}

void GameObject::SetName(const std::string& inName)
{
#if DEBUG_GAMEOBJECT_NAMES
    myName = inName;
#endif
}

const std::string& GameObject::GetName() const
{
#if DEBUG_GAMEOBJECT_NAMES
    return myName;
#else
    static std::string empty;
    return empty;
#endif
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

TagMask GameObject::GetTags() const
{
    return myTags;
}

void GameObject::AddTags(TagMask inTags)
{
    myTags |= inTags;
}

void GameObject::RemoveTags(TagMask inTags)
{
    myTags &= ~inTags;
}

void GameObject::ClearTags()
{
    myTags = 0;
}

bool GameObject::HasAnyTag(TagMask inTags) const
{
    return myTags & inTags;
}

bool GameObject::HasAllTags(TagMask inTags) const
{
    return myTags == inTags;
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