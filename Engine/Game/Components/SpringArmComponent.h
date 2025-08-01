#pragma once
#include "Engine/ComponentSystem/Component.h"
#include "Engine/ComponentSystem/GameObjectTag.hpp"

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
    META(SerializeField)
    float myLength = 100;

    // Offset from components position.
    META(SerializeField)
    glm::vec3 myOffset = glm::vec3(0, 0, 0);

    META(SerializeField)
    TagMask myExclusionTags = 0;

    // Distance to move the attached object in towards the spring arm compared to the hit position.
    META(SerializeField)
    float myHitOffset = 0;
};
