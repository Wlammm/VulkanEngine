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
}

World::~World()
{
	del(myAssetRegistry);
}

void World::Init()
{
	auto entity = myRegistry.create();

	{
		auto& camera = myRegistry.emplace<Camera>(entity);
		camera.CreatePerspective(VulkanContext::GetRenderResolution());
		auto& transform = myRegistry.emplace<Transform>(entity);
		transform.SetPosition(0, 0, -10);
	}

	entity = myRegistry.create();
	auto& transform = myRegistry.emplace<Transform>(entity);

	Model::CreateInfo createInfo{};
	createInfo.InvertY = true;
	auto& staticMesh = myRegistry.emplace<StaticMesh>(entity, "Assets/Tree.fbx", createInfo);
}

entt::registry& World::GetRegistry()
{
	return myRegistry;
}

AssetRegistry& World::GetAssetRegistry()
{
	return *myAssetRegistry;
}
