#pragma once
#include "Engine/Components/CameraComponent.h"
#include "Engine/ComponentSystem/Actor.h"
#include "Game/Components/SpringArmComponent.h"
#include "Game/Components/Player/PlayerCameraControllerComponent.h"

class PlayerCameraActor : public Actor
{
public:
    PlayerCameraActor();

    void OnCreate() override;
    
private:
    META(SerializeField)
    CameraComponent myCamera;
    
    META(SerializeField)
    PlayerCameraControllerComponent myPlayerCameraController;
    
    META(SerializeField)
    SpringArmComponent mySpringArmComponent;
};
