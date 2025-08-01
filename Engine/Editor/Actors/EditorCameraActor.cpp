#include "EditorPch.h"
#include "EditorCameraActor.h"

#include "Engine/Engine.h"

EditorCameraActor::EditorCameraActor()
{
    myCamera.CreatePerspective(Engine::GetRenderResolution());
}

EditorCameraMovementComponent& EditorCameraActor::GetMovementComponent()
{
    return myMovementComponent;
}

CameraComponent& EditorCameraActor::GetCameraComponent()
{
    return myCamera;
}
