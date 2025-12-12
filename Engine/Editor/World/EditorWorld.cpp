#include "EditorPch.h"
#include "EditorWorld.h"

#include "Actors/EditorCameraActor.h"
#include "Engine/Engine.h"

void EditorWorld::Init()
{
    myEditorCamera = SpawnActor<EditorCameraActor>("EditorCamera");
    SetMainCamera(&myEditorCamera->GetCameraComponent());
}

void EditorWorld::Tick()
{
    ZoneScoped;
	
    for(const UniquePtr<WorldSystem>& system : mySystemManager->GetAllSystems())
    {
        system->EditorTick();
    }

    for (const UniquePtr<Actor>& actor : myActors)
    {
        actor->DoEditorTick();
        actor->TickRenderState();
    }
}

EditorCameraActor* EditorWorld::GetEditorCamera() const
{
    return myEditorCamera;
}
