#include "GamePch.h"
#include "PlayerActor.h"

#include "Game/GameTags.h"

PlayerActor::PlayerActor()
    : myStaticMesh("Assets/Characters/PlayerCharacter/Character.fbx")
{
    AddTags(GameTags::Player);
}

void PlayerActor::OnCreate()
{
    Actor::OnCreate();

    myStaticMesh.SetMaterialAsync("Assets/Characters/PlayerCharacter/CharacterMaterial.mat", 0);
}
