#include "EnginePch.h"
#include "EditorCameraMovementComponent.h"

#include "Engine.h"
#include "TransformComponent.h"
#include "Core/Input.h"

void EditorCameraMovementComponent::Tick()
{
    glm::vec3 movement{};

    if(Input::IsKeyPressed(KeyCode::W))
    {
        movement += GetTransform().GetForward();
    }

    if (Input::IsKeyPressed(KeyCode::S))
    {
        movement -= GetTransform().GetForward();
    }

    if (Input::IsKeyPressed(KeyCode::D))
    {
        movement += GetTransform().GetRight();
    }

    if (Input::IsKeyPressed(KeyCode::A))
    {
        movement -= GetTransform().GetRight();
    }

    if (Input::IsKeyPressed(KeyCode::E))
    {
        movement += GetTransform().GetUp();
    }

    if (Input::IsKeyPressed(KeyCode::Q))
    {
        movement -= GetTransform().GetUp();
    }

    if (Input::IsKeyPressed(KeyCode::LeftShift))
        movement *= myShiftMultiplier;

    if (Input::IsKeyDown(MouseButton::ScrollForward))
        myMovementSpeed *= myScrollMultiplier;

    if (Input::IsKeyDown(MouseButton::ScrollBackwards))
        myMovementSpeed /= myScrollMultiplier;

    GetTransform().Move(movement * myMovementSpeed);

    if (Input::IsKeyPressed(MouseButton::Right))
    {
        glm::vec2 mouseDelta = Input::GetMouseDelta() / Engine::GetRenderResolution();

        static float yaw = GetTransform().GetRotationRad().y;
        static float pitch = GetTransform().GetRotationRad().x;

        yaw += mouseDelta.x * myMouseSensitivity;
        pitch += mouseDelta.y * myMouseSensitivity;
        pitch = glm::clamp(-glm::pi<float>() * 0.5f + 0.001f, glm::pi<float>() * 0.5f - 0.001f, pitch);

        GetTransform().SetRotationRad({ pitch, yaw, 0.0f });
    }
}