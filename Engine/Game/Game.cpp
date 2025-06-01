#include "GamePch.h"
#include "Game.h"

#include "Components/Player/PlayerControllerComponent.h"
#include "Components/Player/SpringArmComponent.h"
#include "Engine/Engine.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/CapsuleColliderComponent.h"
#include "Engine/Components/StaticMeshComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/ComponentSystem/ComponentSystem.h"
#include "Engine/ComponentSystem/GameObject.h"
#include "Engine/World/World.h"

Game::Game()
{
    check(myInstance == nullptr);
    myInstance = this;

    GameObject* player = Engine::GetWorld().GetComponentSystem().CreateGameObject();
    player->GetTransform()->SetPosition(glm::vec3{0, 5000, 0});
    player->AddComponent<CapsuleColliderComponent>();
    player->GetTransform()->SetRotationDeg(glm::vec3{0, 0, 0});
    RigidbodyComponent* rigidbody = player->AddComponent<RigidbodyComponent>();
    rigidbody->SetRotationConstraint(true, true, true);
    player->AddComponent<PlayerControllerComponent>();
    StaticMeshComponent* staticMesh = player->AddComponent<StaticMeshComponent>("Assets/Primitives/Capsule.fbx");

    GameObject* camera =  Engine::GetWorld().GetComponentSystem().CreateGameObject();
    CameraComponent* cameraComp = camera->AddComponent<CameraComponent>();
    camera->AddComponent<SpringArmComponent>()->SetLength(200);
    camera->GetTransform()->SetParent(player->GetTransform());
}

Game::~Game()
{
    myInstance = nullptr;
}

void Game::StaticTick()
{
    myInstance->Tick();
}

void Game::Tick()
{
}
