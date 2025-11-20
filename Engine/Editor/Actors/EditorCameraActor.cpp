#include "EditorPch.h"
#include "EditorCameraActor.h"

#include "Engine/Engine.h"

EditorCameraActor::EditorCameraActor()
{
    myCamera.CreatePerspective(Engine::GetRenderResolution());
}

void EditorCameraActor::OnCreate()
{
    Actor::OnCreate();
    
    myCamera.SetAsMainCamera();
}

bool EditorCameraActor::IsTransientActor() const
{
    return true;
}

EditorCameraMovementComponent& EditorCameraActor::GetMovementComponent()
{
    return myMovementComponent;
}

CameraComponent& EditorCameraActor::GetCameraComponent()
{
    return myCamera;
}
