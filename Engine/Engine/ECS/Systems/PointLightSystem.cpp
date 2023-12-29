#include "EnginePch.h"
#include "PointLightSystem.h"
#include "Engine.h"
#include "World/World.h"
#include "Core/Time.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/PointLight.h"

PointLightSystem::PointLightSystem()
{
	SetDependencies<Transform, const PointLight>();
}

void PointLightSystem::Tick()
{
	auto view = Engine::GetWorld().GetRegistry().view<Transform, const PointLight>();

	for (auto [ent, transform, light] : view.each())
	{
		constexpr float radius = 300;
		transform.SetPositionX(cos(Time::GetTimeSinceStart() + light.myRange) * radius);
		transform.SetPositionZ(sin(Time::GetTimeSinceStart() + light.myRange) * radius);
	}
}
