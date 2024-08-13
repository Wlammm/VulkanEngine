#pragma once
#include "ComponentSystem/Component.h"

class VulkanImage;
class DirectionalLightComponent : public Component
{
public:
    DirectionalLightComponent();
    ~DirectionalLightComponent();

    void SetColor(const glm::vec4& inColor);
    
    const glm::vec4& GetColor() const;
    const glm::mat4& GetLightView() const;
    const glm::mat4& GetLightProjection() const;

    bool IsShadowsEnabled() const;
    void EnableShadows();

private:
    bool myIsShadowsEnabled = false;
    
    glm::vec4 myColor = {1, 1, 1, 1};
    glm::mat4 myLightView;
    glm::mat4 myLightProjection;
};