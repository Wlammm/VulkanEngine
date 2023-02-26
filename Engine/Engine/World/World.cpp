#include "World.h"
#include "Math/Camera.h"
import VulkanContext;
#include "Math/Transform.h"

World::World()
{
	auto entity = myRegistry.create();

	auto& camera = myRegistry.emplace<Camera>(entity);
	camera.CreatePerspective(VulkanContext::GetRenderResolution());
	auto& transform = myRegistry.emplace<Transform>(entity);
}

World::~World()
{

}

const entt::registry& World::GetRegistry() const
{
	return myRegistry;
}