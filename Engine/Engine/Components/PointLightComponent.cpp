#include "EnginePch.h"
#include "PointLightComponent.h"

#include "TransformComponent.h"
#include "Engine/Engine.h"
#include "Engine/Rendering/SharedWithShaders/MeshStructs.hpp"
#include "Engine/Systems/PointLightSystem.h"

PointLightComponent::PointLightComponent()
{
    
}

void PointLightComponent::OnCreate()
{
    MarkRenderStateDirty();
}

void PointLightComponent::OnDestroy()
{
    Component::OnDestroy();
    
    if (myPointLightInstanceIndex != -1)
        Engine::GetEngineSystem<PointLightSystem>().RemoveLight(myPointLightInstanceIndex);
}

void PointLightComponent::OnRenderStateDirty()
{
    Component::OnRenderStateDirty();

    PointLightData data{};
    // TODO: Make color be a vec3 instead and pack intensity into alpha..
    data.myColor = glm::vec4(myColor, 1.0f);
    data.myIntensity = myIntensity;
    data.myRange = myRange;
    data.myPosition = GetTransform().GetPosition();
    
    if (myPointLightInstanceIndex == -1)
        myPointLightInstanceIndex = Engine::GetEngineSystem<PointLightSystem>().AddLight(data);
    else
        Engine::GetEngineSystem<PointLightSystem>().UpdateLight(myPointLightInstanceIndex, data);
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