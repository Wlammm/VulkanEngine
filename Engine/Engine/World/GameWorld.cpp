#include "EnginePch.h"
#include "GameWorld.h"

#include "Engine/Engine.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/ComponentSystem/Actor.h"
#include "Game/Actors/PlayerActor.h"
#include "Game/Actors/PlayerCameraActor.h"

void GameWorld::Init()
{
    World::Init();
    
    PlayerActor* player = SpawnActor<PlayerActor>("Player");
    player->GetTransform().SetPosition(glm::vec3{0, 5000, 0});
    player->GetTransform().SetRotationDeg(glm::vec3{0, 0, 0});

    PlayerCameraActor* camera = SpawnActor<PlayerCameraActor>("PlayerCamera");
    camera->GetTransform().SetParent(player->GetTransform());
}

void GameWorld::Tick()
{
    ZoneScoped;
	
    for(UniquePtr<WorldSystem>& system : mySystemManager->GetAllSystems())
    {
        system->Tick();
    }

    for (const UniquePtr<Actor>& actor : myActors)
    {
        actor->DoTick();
    }
    
    for (const UniquePtr<Actor>& actor : myActors)
    {
        actor->TickRenderState();
    }
}
