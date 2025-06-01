#include "GamePch.h"
#include "SpringArmComponent.h"

#include "Engine/Components/TransformComponent.h"
#include "Engine/World/World.h"

SpringArmComponent::SpringArmComponent()
{
}

void SpringArmComponent::SetLength(const float inLength)
{
    myLength = inLength;
}

void SpringArmComponent::Tick()
{
    Component::Tick();

    if (!GetTransform()->GetParent())
        return;
    
    RaycastHit hit;
    bool hitFound = GetWorld()->Raycast(GetTransform()->GetParent()->GetPosition(), GetTransform()->GetForward(), hit, myLength);

    if (!hitFound)
    {
        GetTransform()->SetPosition(GetTransform()->GetParent()->GetPosition() + GetTransform()->GetForward() * myLength);
        return;
    }
    
    GetTransform()->SetPosition(hit.myHitPosition - GetTransform()->GetForward() * myHitOffset);
}