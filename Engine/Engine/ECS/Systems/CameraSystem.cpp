#include "EnginePch.h"
#include "CameraSystem.h"
#include "Engine.h"
#include "World/World.h"
#include "Core/Input.h"

CameraSystem::CameraSystem()
{
}

void CameraSystem::Tick()
{
	constexpr float speed = 0.001f;

	auto view = Engine::GetWorld().GetRegistry().view<Camera, Transform>();

	for(auto [ent, cam, transform] : view.each())
	{
		if(Input::IsKeyPressed(KeyCode::W))
		{
			transform.Move(transform.GetForward() * speed);
		}

		if (Input::IsKeyPressed(KeyCode::S))
		{
			transform.Move(-transform.GetForward() * speed);
		}

		if (Input::IsKeyPressed(KeyCode::D))
		{
			transform.Move(transform.GetRight() * speed);
		}

		if (Input::IsKeyPressed(KeyCode::A))
		{
			transform.Move(-transform.GetRight() * speed);
		}
	}
}
