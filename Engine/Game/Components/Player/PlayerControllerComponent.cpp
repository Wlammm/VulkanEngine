#include "GamePch.h"
#include "PlayerControllerComponent.h"

#include "Engine/Components/TransformComponent.h"
#include "Engine/Core/Input.h"

PlayerControllerComponent::PlayerControllerComponent()
{
    
}

void PlayerControllerComponent::Tick()
{
    Component::Tick();

    glm::vec3 movement{};

    if (Input::IsKeyDown(KeyCode::W))
    {
        movement += GetTransform()->GetForward();
    }
    if (Input::IsKeyDown(KeyCode::S))
    {
        movement += -GetTransform()->GetForward();
    }
    if (Input::IsKeyDown(KeyCode::A))
    {
        movement += GetTransform()->GetRight();
    }
    if (Input::IsKeyDown(KeyCode::D))
    {
        movement += -GetTransform()->GetRight();
    }

    movement.y = 0;
    if (length(movement) != 0)
        movement = glm::normalize(movement);

    GetTransform()->Move(movement);
}
