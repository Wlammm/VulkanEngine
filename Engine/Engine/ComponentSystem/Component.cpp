#include "EnginePch.h"
#include "Component.h"

#include "GameObject.h"
#include "Engine/Components/TransformComponent.h"

void Component::MarkRenderStateDirty()
{
    myGameObject->MarkRenderStateDirty();
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
