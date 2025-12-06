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
    META(SerializeField)
    StaticMeshComponent myStaticMesh;
    
    META(SerializeField)
    PlayerComponent myPlayerComponent;
    
    META(SerializeField)
    CharacterControllerComponent myCharacterController;
};
