#include "EnginePch.h"
#include "World.h"
#include "Vulkan/VulkanContext.h"
#include "Assets/AssetRegistry.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/StaticMesh.h"
#include "Tracy/tracy/Tracy.hpp"
#include "ECS/Components/PointLight.h"
#include "ECS/Components/DirectionalLight.h"

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
	ZoneScoped;
	auto entity = myRegistry.create();

	{
		auto& camera = myRegistry.emplace<Camera>(entity);
		camera.CreatePerspective(VulkanContext::GetRenderResolution(), 90.0f, 100.0f);
		auto& transform = myRegistry.emplace<Transform>(entity);
		transform.SetPosition(0, 0, -10);
	}

	{
		entity = myRegistry.create();
		auto& transform = myRegistry.emplace<Transform>(entity);
		transform.SetScale(1);
		transform.SetRotationDeg(0, 0, 0);
	
		auto& staticMesh = myRegistry.emplace<StaticMesh>(entity, "Assets/Sponza/NewSponza_Main_Yup_002.fbx");
	}

	{
		entity = myRegistry.create();
		auto& transform = myRegistry.emplace<Transform>(entity);
		transform.SetScale(1);
		transform.SetRotationDeg(0, 0, 0);
		transform.SetPosition(0, 150, 0);

		auto& light = myRegistry.emplace<PointLight>(entity);
		light.myColor = Color(0, 8, 10, 1);
		light.myRange = 500.0f;
	}

	{
		entity = myRegistry.create();
		auto& transform = myRegistry.emplace<Transform>(entity);
		transform.SetScale(1);
		transform.SetRotationDeg(0, 0, 0);
		transform.SetPosition(0, 150, 0);

		auto& light = myRegistry.emplace<PointLight>(entity);
		light.myColor = Color(0, 8, 0, 1);
		light.myRange = 500 + PI;
	}

	{
		entity = myRegistry.create();
		auto& transform = myRegistry.emplace<Transform>(entity);
		transform.SetScale(1);
		transform.SetRotationDeg(-90, 0, 0);

		auto& light = myRegistry.emplace<DirectionalLight>(entity);
		light.myColor = Color(1, 1, 1, 1);
	}
}

entt::registry& World::GetRegistry()
{
	return myRegistry;
}

AssetRegistry& World::GetAssetRegistry()
{
	return *myAssetRegistry;
}
