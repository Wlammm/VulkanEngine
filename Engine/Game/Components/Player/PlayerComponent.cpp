#include "GamePch.h"
#include "PlayerComponent.h"

#include "Engine/Components/CharacterControllerComponent.h"
#include "Engine/Components/RigidbodyComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/ComponentSystem/Actor.h"
#include "Engine/Core/Input.h"

PlayerComponent::PlayerComponent()
{
    
}

void PlayerComponent::TickPhysics()
{
    Component::TickPhysics();
    
    glm::vec3 movement{};

    if (Input::IsKeyPressed(KeyCode::W))
    {
        movement += GetTransform().GetForward();
    }
    if (Input::IsKeyPressed(KeyCode::S))
    {
        movement -= GetTransform().GetForward();
    }
    if (Input::IsKeyPressed(KeyCode::A))
    {
        movement -= GetTransform().GetRight();
    }
    if (Input::IsKeyPressed(KeyCode::D))
    {
        movement += GetTransform().GetRight();
    }

    movement.y = 0;
    if (length(movement) != 0)
    {
        CharacterControllerComponent* characterController = GetActor()->GetComponent<CharacterControllerComponent>();
        if(!characterController)
            return;

        movement = glm::normalize(movement);

        if (Input::IsKeyPressed(KeyCode::LeftShift))
            movement *= mySprintSpeed;
        else
            movement *= mySpeed;
        
        characterController->Move(movement);
    }

    if (Input::IsKeyDown(KeyCode::Space))
    {
        CharacterControllerComponent* characterController = GetActor()->GetComponent<CharacterControllerComponent>();
        if(!characterController)
            return;

        if (characterController->IsGrounded())
            characterController->Jump(myJumpForce);
    }
}
