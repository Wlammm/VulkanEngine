#include "EnginePch.h"
#include "EditorCameraMovementComponent.h"

#include "Engine.h"
#include "TransformComponent.h"
#include "Core/Input.h"

void EditorCameraMovementComponent::Tick()
{
    UpdateMovement();
    UpdateRotation();
}

void EditorCameraMovementComponent::ResetMouseDelta()
{
    myMouseDelta = glm::vec2(0.0f, 0.0f);
    myResetMouseDelta = true;
}

void EditorCameraMovementComponent::UpdateMovement()
{
    glm::vec3 movement{};

    if (Input::IsKeyPressed(KeyCode::W))
    {
        movement += GetTransform()->GetForward();
    }

    if (Input::IsKeyPressed(KeyCode::S))
    {
        movement -= GetTransform()->GetForward();
    }

    if (Input::IsKeyPressed(KeyCode::D))
    {
        movement += GetTransform()->GetRight();
    }

    if (Input::IsKeyPressed(KeyCode::A))
    {
        movement -= GetTransform()->GetRight();
    }

    if (Input::IsKeyPressed(KeyCode::E))
    {
        movement += GetTransform()->GetUp();
    }

    if (Input::IsKeyPressed(KeyCode::Q))
    {
        movement -= GetTransform()->GetUp();
    }

    if (Input::IsKeyPressed(KeyCode::LeftShift))
        movement *= myShiftMultiplier;

    if (Input::IsKeyDown(MouseButton::ScrollForward))
        myMovementSpeed *= myScrollMultiplier;

    if (Input::IsKeyDown(MouseButton::ScrollBackwards))
        myMovementSpeed /= myScrollMultiplier;

    GetTransform()->Move(movement * myMovementSpeed);
}

void EditorCameraMovementComponent::UpdateRotation()
{
    auto& io = ImGui::GetIO();

    if (Input::IsKeyPressed(MouseButton::Right))
    {
        static bool reset = false;
        if (!reset)
        {
            myMouseDelta = Input::GetMouseDelta() / Engine::GetRenderResolution();

            myYaw += myMouseDelta.x * myMouseSensitivity;
            myPitch += myMouseDelta.y * myMouseSensitivity;
        }

        if (reset)
        {
            reset = false;
        }

        if (myResetMouseDelta)
        {
            reset = true;
            myResetMouseDelta = false;
        }

        myPitch = glm::clamp(myPitch, -glm::pi<float>() * 0.5f + 0.001f, glm::pi<float>() * 0.5f - 0.001f);
        GetTransform()->SetRotationRad({ myPitch, myYaw, 0.0f });
    }
}