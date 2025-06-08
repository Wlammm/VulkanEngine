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
    
    GetTransform()->SetRotationLocal(glm::quat({myPitch, myYaw, 0}));
}
