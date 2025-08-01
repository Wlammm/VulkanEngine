#pragma once
#include "Engine/Components/CharacterControllerComponent.h"
#include "Engine/Components/StaticMeshComponent.h"
#include "Engine/ComponentSystem/Actor.h"
#include "Game/Components/Player/PlayerComponent.h"

class PlayerActor : public Actor
{
public:
    PlayerActor();

    void OnCreate() override;

private:
    StaticMeshComponent myStaticMesh;
    PlayerComponent myPlayerComponent;
    CharacterControllerComponent myCharacterController;
};
