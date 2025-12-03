#pragma once
#include "Engine/Components/DirectionalLightComponent.h"
#include "Engine/ComponentSystem/Actor.h"

class DirectionalLightActor : public Actor
{
public:
    DirectionalLightComponent& GetDirectionalLightComponent();
    
private:
    META(SerializeField)
    DirectionalLightComponent myDirectionalLight;
};
