#pragma once
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/EditorCameraMovementComponent.h"
#include "Engine/ComponentSystem/Actor.h"

class EditorCameraActor : public Actor
{
public:
    EditorCameraActor();
    
    EditorCameraMovementComponent& GetMovementComponent();
    CameraComponent& GetCameraComponent();

private:
    CameraComponent myCamera;
    EditorCameraMovementComponent myMovementComponent;
};
