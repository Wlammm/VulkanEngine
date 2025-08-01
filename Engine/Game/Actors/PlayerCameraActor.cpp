#include "GamePch.h"
#include "PlayerCameraActor.h"

#include "Engine/Engine.h"
#include "Game/GameTags.h"

PlayerCameraActor::PlayerCameraActor()
{
    myCamera.CreatePerspective(Engine::GetRenderResolution());

    mySpringArmComponent.SetLength(400);
    mySpringArmComponent.SetExclusionTags(GameTags::Player);
    mySpringArmComponent.SetOffset({0, 100, 0});
}

void PlayerCameraActor::OnCreate()
{
    Actor::OnCreate();
    
    myCamera.SetAsMainCamera();
}
