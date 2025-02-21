#include "EnginePch.h"
#include "World.h"

#include "Engine.h"
#include "AssetRegistry/AssetRegistry.h"
#include "Assets/Model.h"
#include "Components/BoxColliderComponent.h"
#include "Components/CameraComponent.h"
#include "Components/CapsuleColliderComponent.h"
#include "Components/ConvexColliderComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/EditorCameraMovementComponent.h"
#include "Components/LandscapeRenderComponent.h"
#include "Components/MeshColliderComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SinWaveMovementComponent.h"
#include "Components/SphereColliderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TransformComponent.h"
#include "Core/Input.h"
#include "Core/Time.h"
#include "Physics/PhysicsSystem.h"
#include "Systems/LandscapeSystem.h"

World::World()
{
	myAssetRegistry = new AssetRegistry();
	mySystemManager = new SystemManager<WorldSystem>();
	CreateWorldSystems();
}

World::~World()
{
	del(mySystemManager);
	del(myAssetRegistry);
}

void World::Init()
{
	// World objects.
	GameObject* camObject = GetComponentSystem().CreateGameObject();
	camObject->GetTransform()->SetPosition(0, 500, -1500);
	CameraComponent* camera = camObject->AddComponent<CameraComponent>();
	camera->CreatePerspective(Engine::GetRenderResolution());
	camObject->AddComponent<EditorCameraMovementComponent>();
	
	GameObject* sponza = GetComponentSystem().CreateGameObject();
	StaticMeshComponent* staticMesh = sponza->AddComponent<StaticMeshComponent>();
	myAssetRegistry->GetAssetAsync<Model>("Assets/Sponza/Sponza.gltf", [staticMesh, sponza](Model* inModel)
	{
		staticMesh->SetModel(inModel);
	});
	sponza->GetTransform()->SetScale(100.0f);
	
	GameObject* landscape = GetComponentSystem().CreateGameObject();
	landscape->AddComponent<LandscapeRenderComponent>();
	
	GameObject* dirLightObject = GetComponentSystem().CreateGameObject();
	DirectionalLightComponent* light = dirLightObject->AddComponent<DirectionalLightComponent>();
	dirLightObject->GetTransform()->SetRotationDeg(321, 314, -50);
	light->SetColor({1, 168/255.0, 120/255.0, 1});

	glm::vec3 startPosition = glm::vec3(-800.0f, 50.0f, -35.0f);
	for (int i = 0; i < 5; i++)
	{
		GameObject* pointLightObject = GetComponentSystem().CreateGameObject();
		PointLightComponent* pointLight = pointLightObject->AddComponent<PointLightComponent>();
		
		const auto& transform = pointLightObject->GetTransform();
		transform->SetPosition(startPosition);
		transform->Move(glm::right() * (i * 400.0f));

		pointLight->SetIntensity(40000.0f);
		pointLight->SetRange(600.0f);
		pointLight->TEMP_SendToGPU();
	}
}

void World::Update()
{
	ZoneScoped;
	
	for(WorldSystem* system : mySystemManager->GetAllSystems())
	{
		system->Tick();
	}
 }

void World::Destroy()
{
	
}

AssetRegistry& World::GetAssetRegistry() const
{
	return *myAssetRegistry;
}

DirectionalLightComponent* World::GetDirectionalLight() const
{
	return GetComponentSystem().GetAnyComponentOfType<DirectionalLightComponent>();
}

ComponentSystem& World::GetComponentSystem() const
{
	return GetWorldSystem<ComponentSystem>();
}

void World::ToggleCactus()
{
	LOG("Toggle cactus");
	if(myCactus)
	{
		myCactus->Destroy();
		myCactus = nullptr;
		return;
	}
	
	myCactus  = GetComponentSystem().CreateGameObject();
	myCactus->GetTransform()->SetPosition(0, 100, 0);
	myAssetRegistry->GetAssetAsync<Model>("Assets/Cactus.fbx", [this](Model* inModel)
	{
		myCactus->AddComponent<StaticMeshComponent>()->SetModel(inModel);
		myCactus->GetTransform()->SetScale(10, 1, 1);
		myCactus->AddComponent<MeshColliderComponent>()->SetModel(inModel);
		myCactus->GetTransform()->SetScale(10, 1, 10);
	});
}

void World::CreateWorldSystems()
{
	mySystemManager->AddSystem<ComponentSystem>(this);
	mySystemManager->AddSystem<LandscapeSystem>(this);
	
	// Keep the physics system last so we make sure we add all object updates before we do any physics calculations.
	mySystemManager->AddSystem<PhysicsSystem>(this);

	mySystemManager->InitAllSystems();
}
