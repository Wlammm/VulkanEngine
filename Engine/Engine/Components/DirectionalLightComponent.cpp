#include "EnginePch.h"
#include "DirectionalLightComponent.h"
#include "Vulkan/VulkanImage.h"

DirectionalLightComponent::DirectionalLightComponent()
{
    EnableShadows();
}

DirectionalLightComponent::~DirectionalLightComponent()
{
}

void DirectionalLightComponent::SetColor(const glm::vec4& inColor)
{
    myColor = inColor;
}

const glm::vec4& DirectionalLightComponent::GetColor() const
{
    return myColor;
}

const glm::mat4& DirectionalLightComponent::GetLightView() const
{
    return myLightView;
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