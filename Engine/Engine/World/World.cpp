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
	light->SetColor({1, 1, 1, 1});

	GameObject* pointLightObject = myComponentSystem->CreateGameObject();
	PointLightComponent* pointLight = pointLightObject->AddComponent<PointLightComponent>();

	//GameObject* cubeObject = myComponentSystem->CreateGameObject();
	//myAssetRegistry->GetAssetAsync<Model>("Assets/Primitives/Cube.fbx", [cubeObject](Model* inModel)
	//{
	//	cubeObject->AddComponent<StaticMeshComponent>()->SetModel(inModel);
	//});

	GameObject* cactus = myComponentSystem->CreateGameObject();
	cactus->GetTransform()->SetPosition(500, 0, 0);
	myAssetRegistry->GetAssetAsync<Model>("Assets/Cactus.fbx", [cactus](Model* inModel)
	{
		cactus->AddComponent<StaticMeshComponent>()->SetModel(inModel);
	});
	
	cactus->AddComponent<SinWaveMovementComponent>();
}

void World::Update()
{
	myComponentSystem->Tick();	
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