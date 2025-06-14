#pragma once
#include "Engine/Components/TransformComponent.h"
#include "Engine/ComponentSystem/Component.h"

class SpringArmComponent : public Component
{
public:
    SpringArmComponent();
    
    void SetLength(const float inLength);
    void SetExclusionTags(const TagMask inTags);

    float GetLength() const;

    void SetOffset(const glm::vec3& inOffset);

    void Tick() override;
    
private:
    float myLength = 100;

    // Offset from components position.
    glm::vec3 myOffset = glm::vec3(0, 0, 0);

    TagMask myExclusionTags = 0;


    // Distance to move the attached object in towards the spring arm compared to the hit position.
    float myHitOffset = 0;
};
