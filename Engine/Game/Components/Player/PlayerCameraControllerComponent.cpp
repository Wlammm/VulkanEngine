#include "GamePch.h"
#include "PlayerCameraControllerComponent.h"

#include "Engine/Engine.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Core/Input.h"
#include "Engine/Utils/Debug.h"

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
        myYaw = 0;
    }

    if (Input::IsKeyUp(KeyCode::LeftAlt))
    {
        myPitch = myStoredPitch;
        myYaw = myStoredYaw;;
    }
    
    if (Input::IsKeyPressed(KeyCode::LeftAlt))
    {
        glm::quat pitchQuat = glm::angleAxis(myPitch, glm::vec3(1, 0, 0));
        glm::quat yawQuat   = glm::angleAxis(myYaw,   glm::vec3(0, 1, 0));
        GetTransform()->SetRotationLocal(yawQuat * pitchQuat); 
    }
    else
    {
        GetTransform()->GetParent()->SetRotationRad({0, myYaw, 0 });
        GetTransform()->SetRotationLocal(glm::quat({myPitch, 0, 0}));
    }
}
