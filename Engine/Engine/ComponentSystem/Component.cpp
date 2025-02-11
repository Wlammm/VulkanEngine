#include "EnginePch.h"
#include "Component.h"

#include "GameObject.h"
#include "Components/TransformComponent.h"

void Component::MarkRenderStateDirty()
{
    myGameObject->MarkRenderStateDirty();
}

void Component::MarkPhysicsStateDirty()
{
    myGameObject->MarkPhysicsStateDirty();
}

GameObject* Component::GetGameObject() const
{
    return myGameObject;
}

TransformComponent* Component::GetTransform() const
{
    return myGameObject->GetTransform();    
}

World* Component::GetWorld() const
{
    return myGameObject->GetWorld();
}
