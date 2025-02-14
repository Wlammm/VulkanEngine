#include "EnginePch.h"
#include "World.h"

#include "Engine.h"
#include "AssetRegistry/AssetRegistry.h"
#include "Assets/Model.h"
#include "Components/BoxColliderComponent.h"
#include "Components/CameraComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/EditorCameraMovementComponent.h"
#include "Components/LandscapeRenderComponent.h"
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

	//GameObject* sponza = GetComponentSystem().CreateGameObject();
	//StaticMeshComponent* staticMesh = sponza->AddComponent<StaticMeshComponent>();
	//myAssetRegistry->GetAssetAsync<Model>("Assets/Sponza/Sponza.gltf", [staticMesh](Model* inModel)
	//{
	//	staticMesh->SetModel(inModel);
	//});
	//sponza->GetTransform()->SetScale(100.0f);
	
	GameObject* dirLightObject = GetComponentSystem().CreateGameObject();
	DirectionalLightComponent* light = dirLightObject->AddComponent<DirectionalLightComponent>();
	dirLightObject->GetTransform()->SetRotationDeg(321, 314, -50);
	light->SetColor({1, 168/255.0, 120/255.0, 1});

	GameObject* platform = GetComponentSystem().CreateGameObject();
	platform->GetTransform()->SetScale(1000, 1, 1000);
	platform->AddComponent<BoxColliderComponent>();
	StaticMeshComponent* staticMesh1 = platform->AddComponent<StaticMeshComponent>();
	myAssetRegistry->GetAssetAsync<Model>("Assets/Primitives/Cube.fbx", [staticMesh1](Model* inModel)
	{
		staticMesh1->SetModel(inModel);
	});

	{
		GameObject* platform1 = GetComponentSystem().CreateGameObject();
		platform1->GetTransform()->SetPositionY(1000);
		platform1->AddComponent<BoxColliderComponent>();
		platform1->AddComponent<RigidbodyComponent>();
		StaticMeshComponent* staticMesh2 = platform1->AddComponent<StaticMeshComponent>();
		myAssetRegistry->GetAssetAsync<Model>("Assets/Primitives/Cube.fbx", [staticMesh2](Model* inModel)
		{
			staticMesh2->SetModel(inModel);
		});
	}

	{
		GameObject* platform1 = GetComponentSystem().CreateGameObject();
		platform1->GetTransform()->SetPositionY(150);
		platform1->AddComponent<BoxColliderComponent>();
		platform1->AddComponent<RigidbodyComponent>();
		StaticMeshComponent* staticMesh2 = platform1->AddComponent<StaticMeshComponent>();
		myAssetRegistry->GetAssetAsync<Model>("Assets/Primitives/Cube.fbx", [staticMesh2](Model* inModel)
		{
			staticMesh2->SetModel(inModel);
		});
	}

	GameObject* sphere1 = GetComponentSystem().CreateGameObject();
	sphere1->GetTransform()->SetPositionY(1500);
	sphere1->AddComponent<SphereColliderComponent>();
	sphere1->AddComponent<RigidbodyComponent>();
	StaticMeshComponent* staticMesh3 = sphere1->AddComponent<StaticMeshComponent>();
	myAssetRegistry->GetAssetAsync<Model>("Assets/Primitives/Sphere.fbx", [staticMesh3](Model* inModel)
	{
		staticMesh3->SetModel(inModel);
	});
	
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

	myAssetRegistry->GetAssetAsync<Model>("Assets/Primitives/Cube.fbx", [this](Model* inModel)
		{
			for(int x  = 0; x < 25; ++x)
			for(int y  = 0; y < 25; ++y)
			for(int z  = 0; z < 25; ++z)
		
						//if(timeSinceSpawn * multiplier > 0.1f && !stopSpawn)
					{
						GameObject* platform1 = GetComponentSystem().CreateGameObject();
						//platform1->GetTransform()->SetPosition(sin((float)Time::GetSeconds() * 4 * multiplier) * 500.f, 1000.f, cos((float)Time::GetSeconds()  * 4 * multiplier) * 500.f);
						platform1->GetTransform()->SetPosition(x * 400.f, y * 400.f, z * 400.f);
						platform1->AddComponent<BoxColliderComponent>();
						platform1->AddComponent<RigidbodyComponent>();
						StaticMeshComponent* staticMesh2 = platform1->AddComponent<StaticMeshComponent>();
						//myAssetRegistry->GetAssetAsync<Model>("Assets/Primitives/Cube.fbx", [staticMesh2](Model* inModel)
						//{
						//	staticMesh2->SetModel(inModel);
						//});
						staticMesh2->SetModel(inModel);
					}
			});

	
	//GameObject* cubeObject = GetComponentSystem().CreateGameObject();
	//myAssetRegistry->GetAssetAsync<Model>("Assets/Primitives/Cube.fbx", [cubeObject](Model* inModel)
	//{
	//	cubeObject->AddComponent<StaticMeshComponent>()->SetModel(inModel);
	//});

	//GameObject* landscape = GetComponentSystem().CreateGameObject();
	//landscape->GetTransform()->SetPosition(0, 0, 0);
	//landscape->AddComponent<LandscapeRenderComponent>();
	
	//ToggleCactus();
}

void World::Update()
{
	ZoneScoped;

	static bool isTrigger = false;
	if(Input::IsKeyDown(KeyCode::K))
	{
		LOG("Pressed");
		isTrigger = !isTrigger;
		for(const BoxColliderComponent& collider : GetComponentSystem().GetAllComponentsOfType<BoxColliderComponent>())
		{
			collider.SetIsTrigger(isTrigger);
		}
	}
	
	for(WorldSystem* system : mySystemManager->GetAllSystems())
	{
		system->Tick();
	}
	
 	if(Input::IsKeyDown(KeyCode::Y))
 	{
 		ToggleCactus();
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
	});
	myCactus->AddComponent<RigidbodyComponent>();
}

void World::CreateWorldSystems()
{
	mySystemManager->AddSystem<ComponentSystem>(this);
	mySystemManager->AddSystem<LandscapeSystem>(this);
	
	// Keep the physics system last so we make sure we add all object updates before we do any physics calculations.
	mySystemManager->AddSystem<PhysicsSystem>(this);

	mySystemManager->InitAllSystems();
}
