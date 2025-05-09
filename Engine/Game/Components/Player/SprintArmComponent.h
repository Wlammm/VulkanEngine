#pragma once
#include "Engine/ComponentSystem/Component.h"

class SprintArmComponent : public Component
{
public:
    SprintArmComponent();
    
    void AttachGameObject(GameObject* inGameObject);
    void SetLength(const float inLength);

    void Tick() override;
    
private:
    GameObject* myGameObject = nullptr;

    float myLength = 100;

    // Offset from components position.
    glm::vec3 myOffset = glm::vec3(0, 0, 0);


    // Distance to move the attached object in towards the spring arm compared to the hit position.
    float myHitOffset = 0;
};
