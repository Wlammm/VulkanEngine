#include "EnginePch.h"
#include "CameraSystem.h"
#include "Engine.h"
#include "World/World.h"
#include "Core/Input.h"
#include "Math/Maths.hpp"
#include "Vulkan/VulkanContext.h"

CameraSystem::CameraSystem()
{
}

void CameraSystem::Tick()
{
	auto view = Engine::GetWorld().GetRegistry().view<Camera, Transform>();

	for(auto [ent, cam, transform] : view.each())
	{
		Vec3f movement{};

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

		if (Input::IsKeyPressed(KeyCode::Q))
		{
			movement += transform.GetUp();
		}

		if (Input::IsKeyPressed(KeyCode::E))
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
			Vec2f mouseDelta = Input::GetMouseDelta().Cast<float>() / Engine::GetRenderResolution().Cast<float>();

			static float yaw = -transform.GetRotationRad().y;
			static float pitch = -transform.GetRotationRad().x;

			yaw += mouseDelta.x * myMouseSensitivity;
			pitch += mouseDelta.y * myMouseSensitivity;
			pitch = Clamp(-PI*0.5f + 0.001f, PI * 0.5f - 0.001f, pitch);

			transform.SetRotationRad({ pitch, -yaw, 0.0f });
		}
	}
}
