#pragma once
#include "Engine/ComponentSystem/Component.h"

class PointLightComponent : public Component
{
public:
    PointLightComponent();

    void OnCreate() override;
    
    void TEMP_SendToGPU();

    const glm::vec3& GetColor() const;
    void SetColor(const glm::vec3& inColor);

    float GetIntensity() const;
    void SetIntensity(const float inIntensity);

    float GetRange() const;
    void SetRange(const float inRange);
    
private:
    META(ExposeToEditor)
    glm::vec3 myColor{1, 1, 1};

    META(ExposeToEditor)
    float myIntensity = 1.0f;

    META(ExposeToEditor)
    float myRange = 100.0f;
};
