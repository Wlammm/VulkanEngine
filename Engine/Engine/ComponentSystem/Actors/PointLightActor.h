#pragma once
#include "Engine/Components/PointLightComponent.h"
#include "Engine/ComponentSystem/Actor.h"

class PointLightActor : public Actor
{
public:
    PointLightComponent& GetPointLightComponent();

private:
    META(SerializeField)
    PointLightComponent myPointLight;
};
