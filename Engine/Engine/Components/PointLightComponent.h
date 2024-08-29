#pragma once
#include "ComponentSystem/Component.h"

class PointLightComponent : public Component
{
public:
    PointLightComponent();

    void Start() override;
    
    const glm::vec3& GetColor() const;
    void SetColor(const glm::vec3& inColor);

    float GetIntensity() const;
    void SetIntensity(const float inIntensity);

    float GetRange() const;
    void SetRange(const float inRange);
    
private:
    glm::vec3 myColor{1, 1, 0};
    float myIntensity = 1.0f;
    float myRange = 100.0f;
};
