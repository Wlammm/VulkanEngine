#include "EnginePch.h"
#include "DirectionalLightActor.h"

DirectionalLightActor::~DirectionalLightActor()
{
    int a= 10;
}

DirectionalLightComponent& DirectionalLightActor::GetDirectionalLightComponent()
{
    return myDirectionalLight;
}
