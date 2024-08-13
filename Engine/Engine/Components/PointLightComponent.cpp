#include "EnginePch.h"
#include "PointLightComponent.h"

PointLightComponent::PointLightComponent()
{
    
}

const glm::vec3& PointLightComponent::GetColor() const
{
    return myColor;
}

float PointLightComponent::GetIntensity() const
{
    return myIntensity;
}

void PointLightComponent::SetColor(const glm::vec3& inColor)
{
    myColor = inColor;
}

void PointLightComponent::SetIntensity(const float inIntensity)
{
    myIntensity = inIntensity;
}

float PointLightComponent::GetRange() const
{
    return myRange;
}

void PointLightComponent::SetRange(const float inRange)
{
    myRange = inRange;
}