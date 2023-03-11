#include "EnginePch.h"
#include "World.h"
#include "Math/Camera.h"
#include "Vulkan/VulkanContext.h"
#include "Math/Transform.h"

World::World()
{
	auto entity = myRegistry.create();

	auto& camera = myRegistry.emplace<Camera>(entity);
	camera.CreatePerspective(VulkanContext::GetRenderResolution());
	auto& transform = myRegistry.emplace<Transform>(entity);
	transform.SetPosition(0, 0, -10);
}

World::~World()
{

}

entt::registry& World::GetRegistry()
{
	return myRegistry;
}