#include "EnginePch.h"
#include "World.h"

#include "Engine.h"
#include "Assets/AssetRegistry.h"
#include "Components/CameraComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/EditorCameraMovementComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"

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
	staticMesh->SetModel(myAssetRegistry->GetModel("Assets/Sponza/NewSponza_Main_Yup_002.fbx"));
	//staticMesh->SetModel(myAssetRegistry->GetModel("Assets/Primitives/Cube.fbx"));
	
	GameObject* dirLightObject = myComponentSystem->CreateGameObject();
	DirectionalLightComponent* light = dirLightObject->AddComponent<DirectionalLightComponent>();
	light->SetColor({1, 1, 1, 1});

	GameObject* pointLightObject = myComponentSystem->CreateGameObject();
	PointLightComponent* pointLight = pointLightObject->AddComponent<PointLightComponent>();
	
}

void World::Update()
{
	myComponentSystem->Update();	
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