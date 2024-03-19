#include "EnginePch.h"
#include "CameraSystem.h"
#include "Engine.h"
#include "World/World.h"
#include "Core/Input.h"
#include "Vulkan/VulkanContext.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Camera.h"

CameraSystem::CameraSystem()
{
	SetDependencies<Transform, const Camera>();
}

void CameraSystem::Tick()
{
	auto view = Engine::GetWorld().GetRegistry().view<const Camera, Transform>();

	for(auto [ent, cam, transform] : view.each())
	{
		glm::vec3 movement{};

		if(Input::IsKeyPressed(KeyCode::W))
		{
			movement += transform.GetForward();
		}

		if (Input::IsKeyPressed(KeyCode::S))
		{
			movement -= transform.GetForward();
		}

		if (Input::IsKeyPressed(KeyCode::D))
		{
			movement += transform.GetRight();
		}

		if (Input::IsKeyPressed(KeyCode::A))
		{
			movement -= transform.GetRight();
		}

		if (Input::IsKeyPressed(KeyCode::E))
		{
			movement += transform.GetUp();
		}

		if (Input::IsKeyPressed(KeyCode::Q))
		{
			movement -= transform.GetUp();
		}

		if (Input::IsKeyPressed(KeyCode::LeftShift))
			movement *= myShiftMultiplier;

		if (Input::IsKeyDown(MouseButton::ScrollForward))
			myMovementSpeed *= myScrollMultiplier;

		if (Input::IsKeyDown(MouseButton::ScrollBackwards))
			myMovementSpeed /= myScrollMultiplier;

		transform.Move(movement * myMovementSpeed);

		if (Input::IsKeyPressed(MouseButton::Right))
		{
			glm::vec2 mouseDelta = Input::GetMouseDelta() / Engine::GetRenderResolution();

			static float yaw = transform.GetRotationRad().y;
			static float pitch = transform.GetRotationRad().x;

			yaw += mouseDelta.x * myMouseSensitivity;
			pitch += mouseDelta.y * myMouseSensitivity;
			pitch = glm::clamp(-glm::pi<float>()*0.5f + 0.001f, glm::pi<float>() * 0.5f - 0.001f, pitch);

			transform.SetRotationRad({ pitch, yaw, 0.0f });
		}
	}
}
