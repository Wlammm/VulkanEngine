#include "GamePch.h"
#include "PlayerCameraControllerComponent.h"

#include "Engine/Engine.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/ComponentSystem/Actor.h"
#include "Engine/Core/Input.h"
#include "Engine/Utils/Debug.h"
#include "Game/Components/SpringArmComponent.h"

void PlayerCameraControllerComponent::Tick()
{
    Component::Tick();

    glm::vec2 mouseMovement = Input::GetMouseDelta() / Engine::GetRenderResolution();
    mouseMovement *= myMouseSensitivity;

    myPitch += mouseMovement.y;
    myYaw += mouseMovement.x;

    myPitch = glm::clamp(myPitch, -glm::pi<float>() * 0.5f + 0.001f, glm::pi<float>() * 0.5f - 0.001f);

    if (Input::IsKeyDown(KeyCode::LeftAlt))
    {
        myStoredPitch = myPitch;
        myStoredYaw = myYaw;
        myStoredSpringArmLength = GetActor()->GetComponent<SpringArmComponent>()->GetLength();

        // This resets the yaw as its already inheriting the current yaw from the parent. If we'd keep the current value we'd get double the yaw (one from parent and one from the local rotation)
        myYaw = 0;
    }

    if (Input::IsKeyUp(KeyCode::LeftAlt))
    {
        myPitch = myStoredPitch;
        myYaw = myStoredYaw;
        GetActor()->GetComponent<SpringArmComponent>()->SetLength(myStoredSpringArmLength);
    }
    
    if (Input::IsKeyPressed(KeyCode::LeftAlt))
    {
        glm::quat pitchQuat = glm::angleAxis(myPitch, glm::vec3(1, 0, 0));
        glm::quat yawQuat   = glm::angleAxis(myYaw,   glm::vec3(0, 1, 0));
        GetTransform().SetRotationLocal(yawQuat * pitchQuat);

        if (Input::IsKeyDown(MouseButton::ScrollForward))
        {
            SpringArmComponent* springArm = GetActor()->GetComponent<SpringArmComponent>();
            float length = springArm->GetLength();
            length -= mySpringArmChangeAmount;
            length = std::clamp(length, myMinSpringArmLength, myMaxSpringArmLength);
            springArm->SetLength(length);
        }
        if (Input::IsKeyDown(MouseButton::ScrollBackwards))
        {
            SpringArmComponent* springArm = GetActor()->GetComponent<SpringArmComponent>();
            float length = springArm->GetLength();
            length += mySpringArmChangeAmount;
            length = std::clamp(length, myMinSpringArmLength, myMaxSpringArmLength);
            springArm->SetLength(length);
        }
    }
    else
    {
        GetTransform().GetParent()->SetRotationRad({0, myYaw, 0 });
        GetTransform().SetRotationLocal(glm::quat({myPitch, 0, 0}));
    }
}
