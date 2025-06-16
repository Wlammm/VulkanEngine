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
    META(ExposeToEditor)
    float myLength = 100;

    // Offset from components position.
    META(ExposeToEditor)
    glm::vec3 myOffset = glm::vec3(0, 0, 0);

    META(ExposeToEditor)
    TagMask myExclusionTags = 0;

    // Distance to move the attached object in towards the spring arm compared to the hit position.
    META(ExposeToEditor)
    float myHitOffset = 0;
};
