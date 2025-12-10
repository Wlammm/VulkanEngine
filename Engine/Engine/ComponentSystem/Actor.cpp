#include "EnginePch.h"
#include "Actor.h"

#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/Reflection/Type.h"
#include "Engine/World/World.h"

Actor::Actor()
{
}

void Actor::DoOnCreate()
{
    for (Component* comp : myComponents)
    {
        comp->OnCreate();
    }

    OnCreate();
}

void Actor::DoOnDestroy()
{
    for (Component* comp : myComponents)
    {
        comp->OnDestroy();
    }
    
    OnDestroy();
}

void Actor::DoTick()
{
    for (Component* comp : myComponents)
    {
        comp->Tick();
    }
    Tick();
}

void Actor::DoEditorTick()
{
    for (Component* comp : myComponents)
    {
        comp->EditorTick();
    }
}

void Actor::TickRenderState()
{
    if (myRenderStateDirty)
    {
        for (Component* comp : myComponents)
        {
            comp->OnRenderStateDirty();
        }
        myRenderStateDirty = false;
    }
}

void Actor::DoTickPhysics()
{
    for (Component* comp : myComponents)
    {
        comp->TickPhysics();
    }

    TickPhysics();
}

void Actor::DoOnTriggerEnter(Actor* inOther)
{
    for (Component* comp : myComponents)
    {
        comp->OnTriggerEnter(inOther);
    }
    OnTriggerEnter(inOther);
}

void Actor::DoOnTrigger(Actor* inOther)
{
    for (Component* comp : myComponents)
    {
        comp->OnTrigger(inOther);
    }
    OnTrigger(inOther);
}

void Actor::DoOnTriggerExit(Actor* inOther)
{
    for (Component* comp : myComponents)
    {
        comp->OnTriggerExit(inOther);
    }
    OnTriggerExit(inOther);
}

void Actor::DoOnCollisionEnter(Actor* inOther)
{
    for (Component* comp : myComponents)
    {
        comp->OnCollisionEnter(inOther);
    }
    OnCollisionEnter(inOther);
}

void Actor::DoOnCollision(Actor* inOther)
{
    for (Component* comp : myComponents)
    {
        comp->OnCollision(inOther);
    }
    OnCollision(inOther);
}

void Actor::DoOnCollisionExit(Actor* inOther)
{
    for (Component* comp : myComponents)
    {
        comp->OnCollisionExit(inOther);
    }
    OnCollisionExit(inOther);
}

TransformComponent& Actor::GetTransform()
{
    return myTransform;
}

World* Actor::GetWorld() const
{
    return myWorld;
}

void Actor::AddTags(const TagMask inTagMask)
{
    myTagMask |= inTagMask;
}

void Actor::RemoveTags(TagMask inTags)
{
    myTagMask &= ~inTags;
}

void Actor::ClearTags()
{
    myTagMask = 0;
}

TagMask Actor::GetTags() const
{
    return myTagMask;
}

bool Actor::HasAnyTag(const TagMask inTags) const
{
    return myTagMask & inTags;
}

bool Actor::HasAllTags(TagMask inTags) const
{
    return myTagMask == inTags;
}

void Actor::MarkRenderStateDirty()
{
    myRenderStateDirty = true;
}

void Actor::SetName(const std::string& inName)
{
    check(!inName.empty() && "Actor name cannot be empty.");
    myName = inName;
}

const std::string& Actor::GetName() const
{
    return myName;
}

void Actor::Destroy()
{
    myWorld->RemoveActor(this);
}

List<Component*> Actor::GetAllComponents() const
{
    return myComponents;
}

void Actor::RegisterComponentsForType(const Type* inType)
{
    for (const Field& field : inType->GetFields())
    {
        if (field.GetType()->IsA<Component>())
        {
            Component* component = static_cast<Component*>(field.GetPointerToValue(this));
            component->myActor = this;
            myComponents.Add(component);
        }
    }

    for (const Type* type : inType->GetBaseTypes())
    {
        RegisterComponentsForType(type);
    }
}

void Actor::RegisterComponents()
{
    const Type* thisType = ReflectionSystem::GetType(this);
    RegisterComponentsForType(thisType);
}