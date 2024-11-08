#include "EnginePch.h"
#include "World.h"

#include "Engine.h"
#include "AssetRegistry/AssetRegistry.h"
#include "Assets/Model.h"
#include "Components/CameraComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/EditorCameraMovementComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SinWaveMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TransformComponent.h"
#include "Core/Input.h"
#include "Core/Time.h"

World::World()
{
	myAssetRegistry = new AssetRegistry();
	myComponentSystem = new ComponentSystem();
}

World::~World()
{
	del(myComponentSystem);
	del(myAssetRegistry);
}

void World::Init()
{
	GameObject* camObject = myComponentSystem->CreateGameObject();
	CameraComponent* camera = camObject->AddComponent<CameraComponent>();
	camera->CreatePerspective(Engine::GetRenderResolution());
	camObject->AddComponent<EditorCameraMovementComponent>();

	GameObject* sponza = myComponentSystem->CreateGameObject();
	StaticMeshComponent* staticMesh = sponza->AddComponent<StaticMeshComponent>();
	myAssetRegistry->GetAssetAsync<Model>("Assets/Sponza/Sponza.gltf", [staticMesh](Model* inModel)
	{
		staticMesh->SetModel(inModel);
	});
	sponza->GetTransform()->SetScale(100.0f);
	
	GameObject* dirLightObject = myComponentSystem->CreateGameObject();
	DirectionalLightComponent* light = dirLightObject->AddComponent<DirectionalLightComponent>();
	dirLightObject->GetTransform()->SetRotationDeg(321, 314, -50);
	light->SetColor({1, 168/255.0, 120/255.0, 1});

	glm::vec3 startPosition = glm::vec3(-800.0f, 50.0f, -35.0f);
	for (int i = 0; i < 5; i++)
	{
		GameObject* pointLightObject = myComponentSystem->CreateGameObject();
		PointLightComponent* pointLight = pointLightObject->AddComponent<PointLightComponent>();
		
		const auto& transform = pointLightObject->GetTransform();
		transform->SetPosition(startPosition);
		transform->Move(glm::right() * (i * 400.0f));

		pointLight->SetIntensity(40000.0f);
		pointLight->SetRange(600.0f);
		pointLight->TEMP_SendToGPU();
	}
	

	//GameObject* cubeObject = myComponentSystem->CreateGameObject();
	//myAssetRegistry->GetAssetAsync<Model>("Assets/Primitives/Cube.fbx", [cubeObject](Model* inModel)
	//{
	//	cubeObject->AddComponent<StaticMeshComponent>()->SetModel(inModel);
	//});

	
	
	ToggleCactus();
}

void World::Update()
{
	myComponentSystem->Tick();

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
	return myComponentSystem->GetAnyComponentOfType<DirectionalLightComponent>();
}

ComponentSystem& World::GetComponentSystem() const
{
	return *myComponentSystem;	
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
	
	myCactus  = myComponentSystem->CreateGameObject();
	myCactus->GetTransform()->SetPosition(500, 0, 0);
	myAssetRegistry->GetAssetAsync<Model>("Assets/Cactus.fbx", [this](Model* inModel)
	{
		myCactus->AddComponent<StaticMeshComponent>()->SetModel(inModel);
	});
	
	myCactus->AddComponent<SinWaveMovementComponent>();
}
