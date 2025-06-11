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

void SpringArmComponent::SetExclusionTags(const TagMask inTags)
{
    myExclusionTags = inTags;
}

float SpringArmComponent::GetLength() const
{
    return myLength;
}

void SpringArmComponent::SetOffset(const glm::vec3& inOffset)
{
    myOffset = inOffset;
}

void SpringArmComponent::Tick()
{
    Component::Tick();

    TransformComponent* parent = GetTransform()->GetParent();
    if (!parent)
        return;

    glm::vec3 startPos = parent->GetPosition() + myOffset;
    glm::vec3 rayDirection = -GetTransform()->GetForward();
    RaycastHit hit;

    bool hitFound = GetWorld()->Raycast(startPos, rayDirection, hit, myLength, myExclusionTags);
    if(hitFound)
    {
        GetTransform()->SetPosition(hit.myHitPosition);
    }
    else
    {
        GetTransform()->SetPosition(startPos + rayDirection * myLength);
    }
}
