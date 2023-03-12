#include "EnginePch.h"
#include "World.h"
#include "Vulkan/VulkanContext.h"
#include "Assets/AssetRegistry.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/StaticMesh.h"

World::World()
{
	myAssetRegistry = new AssetRegistry();

	auto entity = myRegistry.create();

	{
		auto& camera = myRegistry.emplace<Camera>(entity);
		camera.CreatePerspective(VulkanContext::GetRenderResolution());
		auto& transform = myRegistry.emplace<Transform>(entity);
		transform.SetPosition(0, 0, -10);
	}

	entity = myRegistry.create();
	auto& transform = myRegistry.emplace<Transform>(entity);
	auto& staticMesh = myRegistry.emplace<StaticMesh>(entity);
}

World::~World()
{
	del(myAssetRegistry);
}

entt::registry& World::GetRegistry()
{
	return myRegistry;
}

AssetRegistry& World::GetAssetRegistry()
{
	return *myAssetRegistry;
}
