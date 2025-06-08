#include "GamePch.h"
#include "PlayerControllerComponent.h"

#include "Engine/Components/RigidbodyComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Core/Input.h"

PlayerControllerComponent::PlayerControllerComponent()
{
    
}

void PlayerControllerComponent::TickPhysics()
{
    Component::TickPhysics();

    glm::vec3 movement{};

    if (Input::IsKeyPressed(KeyCode::W))
    {
        movement += GetTransform()->GetForward();
    }
    if (Input::IsKeyPressed(KeyCode::S))
    {
        movement -= GetTransform()->GetForward();
    }
    if (Input::IsKeyPressed(KeyCode::A))
    {
        movement -= GetTransform()->GetRight();
    }
    if (Input::IsKeyPressed(KeyCode::D))
    {
        movement += GetTransform()->GetRight();
    }

    movement.y = 0;
    if (length(movement) != 0)
    {
        RigidbodyComponent* rigidbody = GetComponent<RigidbodyComponent>();
        if(!rigidbody)
            return;

        movement = glm::normalize(movement) * mySpeed;
        movement.y = rigidbody->GetVelocity().y;

        rigidbody->SetVelocity(movement);
    }
}
