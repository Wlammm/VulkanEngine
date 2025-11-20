#include "EnginePch.h"
#include "DirectionalLightComponent.h"

#include "TransformComponent.h"
#include "Engine/Vulkan/VulkanImage.h"

DirectionalLightComponent::DirectionalLightComponent()
{
    EnableShadows();
}

DirectionalLightComponent::~DirectionalLightComponent()
{
}

void DirectionalLightComponent::Tick()
{
    Component::Tick();

}

void DirectionalLightComponent::SetColor(const glm::vec4& inColor)
{
    myColor = inColor;
}

void DirectionalLightComponent::SetIntensity(const float inValue)
{
    myColor.a = inValue;
}

const glm::vec4& DirectionalLightComponent::GetColor() const
{
    return myColor;
}

const glm::mat4& DirectionalLightComponent::GetLightProjection() const
{
    return myLightProjection;
}

bool DirectionalLightComponent::IsShadowsEnabled() const
{
    return myIsShadowsEnabled;
}

void DirectionalLightComponent::EnableShadows()
{
    myIsShadowsEnabled = true;
}

void DirectionalLightComponent::OnColorChanged()
{
    MarkRenderStateDirty();
}
