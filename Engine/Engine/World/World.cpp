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
}

World::~World()
{

}

const entt::registry& World::GetRegistry() const
{
	return myRegistry;
}