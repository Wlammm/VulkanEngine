#pragma once
#include "Engine/ComponentSystem/Component.h"

class VulkanImage;
class DirectionalLightComponent : public Component
{
public:
    DirectionalLightComponent();
    ~DirectionalLightComponent();

    void Tick() override;
    
    void SetColor(const glm::vec4& inColor);
    void SetIntensity(const float inValue);
    
    const glm::vec4& GetColor() const;
    const glm::mat4& GetLightProjection() const;

    bool IsShadowsEnabled() const;
    void EnableShadows();
    
private:
    void OnColorChanged();

    META(SerializeField)
    bool myIsShadowsEnabled = false;

    META(SerializeField, ExposeAsColor, OnInspectorChangedEvent(OnColorChanged))
    glm::vec4 myColor = {1, 1, 1, 1};
    glm::mat4 myLightProjection;
};