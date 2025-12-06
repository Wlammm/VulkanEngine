#pragma once
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/EditorCameraMovementComponent.h"
#include "Engine/ComponentSystem/Actor.h"

class EditorCameraActor : public Actor
{
public:
    EditorCameraActor();

    void OnCreate() override;
    
    bool IsTransientActor() const override;
    
    EditorCameraMovementComponent& GetMovementComponent();
    CameraComponent& GetCameraComponent();

private:
    META(SerializeField)
    CameraComponent myCamera;

    META(SerializeField)
    EditorCameraMovementComponent myMovementComponent;
};
