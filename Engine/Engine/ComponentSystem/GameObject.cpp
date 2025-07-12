#include "EnginePch.h"
#include "GameObject.h"

#include "Component.h"
#include "ComponentSystem.h"
#include "Engine/Components/TransformComponent.h"

GameObject::GameObject(const UniqueID& inGameObjectID)
{
    myGameObjectID = inGameObjectID;
}

GameObject::GameObject(const GameObject& inOther)
{
    myGameObjectID = inOther.myGameObjectID;
}

GameObject& GameObject::operator=(const GameObject& inOther)
{
    myGameObjectID = inOther.myGameObjectID;
    return *this;
}

bool GameObject::operator==(const GameObject& inOther) const
{
    return myGameObjectID == inOther.myGameObjectID;
}

bool GameObject::IsRenderStateDirty() const
{
    return GetGameObjectData().myRenderStateDirty;
}

void GameObject::ResetRenderStateDirtyFlag()
{
    GetGameObjectData().myRenderStateDirty = false;
}

void GameObject::SetName(const std::string& inName)
{
#if DEBUG_GAMEOBJECT_NAMES
    GetGameObjectData().myName = inName;
#endif
}

const std::string& GameObject::GetName() const
{
#if DEBUG_GAMEOBJECT_NAMES
    return GetGameObjectData().myName;
#else
    static std::string empty;
    return empty;
#endif
}

List<Component*> GameObject::GetComponents() const
{
    List<Component*> components;

    for(IComponentArray* array : GetComponentSystem()->GetAllComponentArrays())
    {
        if(Component* comp = array->TryGetComponentForGameObject(*this))
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
    GetComponentSystem()->DestroyGameObject(*this);
}

TagMask GameObject::GetTags() const
{
    return GetGameObjectData().myTags;
}

void GameObject::AddTags(TagMask inTags)
{
    GetGameObjectData().myTags |= inTags;
}

void GameObject::RemoveTags(TagMask inTags)
{
    GetGameObjectData().myTags &= ~inTags;
}

void GameObject::ClearTags()
{
    GetGameObjectData().myTags = 0;
}

bool GameObject::HasAnyTag(TagMask inTags) const
{
    return GetGameObjectData().myTags & inTags;
}

bool GameObject::HasAllTags(TagMask inTags) const
{
    return GetGameObjectData().myTags == inTags;
}

void GameObject::MarkRenderStateDirty()
{
    GetGameObjectData().myRenderStateDirty = true;
}

void GameObject::MarkPhysicsStateDirty()
{
    GetGameObjectData().myPhysicsStateDirty = true;
}

World* GameObject::GetWorld() const
{
    return GetComponentSystem()->GetWorld();
}

MulticastDelegate<void(Component*)>& GameObject::GetOnComponentAdded() const
{
    return GetGameObjectData().myOnComponentAdded;
}

MulticastDelegate<void(Component*)>& GameObject::GetOnComponentRemoved() const
{
    return GetGameObjectData().myOnComponentRemoved;
}

ComponentSystem* GameObject::GetComponentSystem() const
{
    return GetGameObjectData().myComponentSystem;
}

GameObjectData& GameObject::GetGameObjectData() const
{
    return ComponentSystem::GetGameObjectData(*this);
}
