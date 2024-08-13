#include "EnginePch.h"
#include "Component.h"

#include "GameObject.h"
#include "Components/TransformComponent.h"

GameObject* Component::GetGameObject() const
{
    return myGameObject;
}

TransformComponent& Component::GetTransform() const
{
    return *myGameObject->GetComponent<TransformComponent>();    
}