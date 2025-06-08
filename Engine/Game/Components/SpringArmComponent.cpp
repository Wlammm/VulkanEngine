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

void SpringArmComponent::Tick()
{
    Component::Tick();

    auto* parent = GetTransform()->GetParent();
    if (!parent)
        return;

    glm::vec3 startPos = parent->GetPosition();
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
    
    //glm::vec3 start = parent->GetPosition();
    //glm::vec3 direction = -glm::normalize(GetTransform()->GetForward()); // Toward camera
    //RaycastHit hit;
    //
    //bool hitFound = GetWorld()->Raycast(start, direction, hit, myLength, myExclusionTags);
    //
    //if (!hitFound) {
    //    GetTransform()->SetPosition(start + direction * myLength);
    //} else {
    //    GetTransform()->SetPosition(hit.myHitPosition + direction * myHitOffset);
    //}
}
