#pragma once
#include "Engine/Components/LandscapeColliderComponent.h"
#include "Engine/Components/LandscapeRenderComponent.h"
#include "Engine/ComponentSystem/Actor.h"

class LandscapeActor : public Actor
{
public:
    

private:
    META(SerializeField)
    LandscapeRenderComponent myLandscapeRenderComponent;

    META(SerializeField)
    LandscapeColliderComponent myLandscapeColliderComponent;
};
