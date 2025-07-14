#include "EnginePch.h"
#include "GameWorld.h"

#include "Engine/Engine.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/CharacterControllerComponent.h"
#include "Engine/Components/StaticMeshComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Game/GameTags.h"
#include "Game/Components/SpringArmComponent.h"
#include "Game/Components/Player/PlayerCameraControllerComponent.h"
#include "Game/Components/Player/PlayerComponent.h"

void GameWorld::Init()
{
    World::Init();
    
    GameObject player = Engine::GetWorld()->GetComponentSystem().CreateGameObject("Player");
    player.GetTransform()->SetPosition(glm::vec3{0, 5000, 0});
    player.GetTransform()->SetRotationDeg(glm::vec3{0, 0, 0});
    CharacterControllerComponent* characterController = player.AddComponent<CharacterControllerComponent>();
    
    player.AddComponent<PlayerComponent>();
    StaticMeshComponent* staticMesh = player.AddComponent<StaticMeshComponent>("Assets/Characters/PlayerCharacter/Character.fbx");
    staticMesh->SetMaterialAsync("Assets/Characters/PlayerCharacter/Character.mat", 0);
    
    player.AddTags(GameTags::Player);
    player.GetTransform()->SetScale({1, 1, 1});

    GameObject camera =  Engine::GetWorld()->GetComponentSystem().CreateGameObject("PlayerCamera");
    camera.GetTransform()->SetParent(player.GetTransform());
    
    CameraComponent* cameraComp = camera.AddComponent<CameraComponent>();
    cameraComp->CreatePerspective(Engine::GetRenderResolution());
    camera.AddComponent<PlayerCameraControllerComponent>();
    cameraComp->SetAsMainCamera();
    
    SpringArmComponent* springArm = camera.AddComponent<SpringArmComponent>();
    springArm->SetLength(400);
    springArm->SetExclusionTags(GameTags::Player);
    springArm->SetOffset({0, 100, 0});
}

void GameWorld::Tick()
{
    ZoneScoped;
	
    for(WorldSystem* system : mySystemManager->GetAllSystems())
    {
        system->Tick();
    }
}
