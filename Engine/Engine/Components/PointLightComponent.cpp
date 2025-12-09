#include "EnginePch.h"
#include "PointLightComponent.h"

#include "Engine/Engine.h"
#include "Engine/Systems/PointLightSystem.h"

PointLightComponent::PointLightComponent()
{
    
}
void PointLightComponent::OnCreate()
{
    TEMP_SendToGPU();
}

void PointLightComponent::TEMP_SendToGPU()
{
    Engine::GetEngineSystem<PointLightSystem>().AddLight(GetTransform(), this);
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