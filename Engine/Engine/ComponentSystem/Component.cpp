#include "EnginePch.h"
#include "Component.h"

#include "Actor.h"

void Component::MarkRenderStateDirty()
{
    if (myActor)
        myActor->MarkRenderStateDirty();
}

TransformComponent& Component::GetTransform() const
{
    return myActor->GetTransform();
}

Actor* Component::GetActor() const
{
    return myActor;
}

World* Component::GetWorld() const
{
    return myActor->GetWorld();
}