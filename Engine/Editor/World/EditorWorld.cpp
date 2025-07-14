#include "EditorPch.h"
#include "EditorWorld.h"

#include "Engine/Engine.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/EditorCameraMovementComponent.h"

void EditorWorld::Init()
{
    World::Init();

    GameObject editorCamera = GetComponentSystem().CreateGameObject("EditorCamera");
    CameraComponent* cameraComponent = editorCamera.AddComponent<CameraComponent>();
    cameraComponent->CreatePerspective(Engine::GetRenderResolution());
    editorCamera.AddComponent<EditorCameraMovementComponent>();
    SetMainCamera(cameraComponent);
}

void EditorWorld::Tick()
{
    ZoneScoped;
	
    for(WorldSystem* system : mySystemManager->GetAllSystems())
    {
        system->EditorTick();
    }
}
