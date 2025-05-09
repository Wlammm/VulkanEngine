#include "GamePch.h"
#include "SprintArmComponent.h"

#include "Engine/World/World.h"

SprintArmComponent::SprintArmComponent()
{
}

void SprintArmComponent::AttachGameObject(GameObject* inGameObject)
{
    myGameObject = inGameObject;
}

void SprintArmComponent::SetLength(const float inLength)
{
    myLength = inLength;
}

void SprintArmComponent::Tick()
{
    Component::Tick();

    
}
