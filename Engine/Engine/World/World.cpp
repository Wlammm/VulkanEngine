#include "EnginePch.h"
#include "World.h"

#include <PxRigidActor.h>
#include <PxScene.h>

#include "Engine/Engine.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Assets/Model.h"
#include "Engine/Components/BoxColliderComponent.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/CapsuleColliderComponent.h"
#include "Engine/Components/ConvexColliderComponent.h"
#include "Engine/ComponentSystem/ComponentSystem.h"
#include "Engine/Components/DirectionalLightComponent.h"
#include "Engine/Components/EditorCameraMovementComponent.h"
#include "Engine/Components/LandscapeColliderComponent.h"
#include "Engine/Components/LandscapeRenderComponent.h"
#include "Engine/Components/MeshColliderComponent.h"
#include "Engine/Components/PointLightComponent.h"
#include "Engine/Components/SinWaveMovementComponent.h"
#include "Engine/Components/SphereColliderComponent.h"
#include "Engine/Components/StaticMeshComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Time.h"
#include "Engine/Physics/PhysicsSystem.h"
#include "Engine/Serialization/BinarySerializer.h"
#include "Engine/Systems/LandscapeSystem.h"

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
	//GameObject* camObject = GetComponentSystem().CreateGameObject();
	//camObject->GetTransform()->SetPosition(0, 500, -1500);
	//CameraComponent* camera = camObject->AddComponent<CameraComponent>();
	//camera->CreatePerspective(Engine::GetRenderResolution());
	//camObject->AddComponent<EditorCameraMovementComponent>();

	{
		GameObject sponza = GetComponentSystem().CreateGameObject("Sponza");
		sponza.GetTransform()->SetPositionY(2000);
		StaticMeshComponent* staticMesh = sponza.AddComponent<StaticMeshComponent>();
		myAssetRegistry->GetAssetAsync<Model>("Assets/Sponza/Sponza.gltf", [staticMesh, sponza](Model* inModel)
		{
			staticMesh->SetModel(inModel);
		});
		sponza.GetTransform()->SetScale(100.0f);
	}

	{
		GameObject sponza = GetComponentSystem().CreateGameObject("Sponza2");
		sponza.GetTransform()->SetPosition(5000, 2000, 0);
		StaticMeshComponent* staticMesh = sponza.AddComponent<StaticMeshComponent>();
		myAssetRegistry->GetAssetAsync<Model>("Assets/Sponza/Sponza.gltf", [staticMesh, sponza](Model* inModel)
		{
			staticMesh->SetModel(inModel);
			//sponza.AddComponent<MeshColliderComponent>()->SetModel(inModel);
		});
		sponza.GetTransform()->SetScale(100.0f);
	}
	
	GameObject landscape = GetComponentSystem().CreateGameObject("Landscape");
	landscape.AddComponent<LandscapeRenderComponent>();
	landscape.AddComponent<LandscapeColliderComponent>();

	GetAssetRegistry().GetAssetAsync<Model>("Assets/Primitives/Cube.fbx", [&](Model* inModel)
	{
		for (int i =0; i < 50; ++i)
		{
			GameObject cube = GetComponentSystem().CreateGameObject("Cube");
			cube.GetTransform()->SetPosition(glm::vec3{(float)i * 100, 10000, 0});
			cube.AddComponent<StaticMeshComponent>()->SetModel(inModel);
			cube.AddComponent<BoxColliderComponent>();
			cube.AddComponent<RigidbodyComponent>();
		}
	});
	
	GameObject dirLightObject = GetComponentSystem().CreateGameObject("DirectionalLight");
	DirectionalLightComponent* light = dirLightObject.AddComponent<DirectionalLightComponent>();
	dirLightObject.GetTransform()->SetRotationDeg(321, 314, -50);
	light->SetColor({1, 168/255.0, 120/255.0, 1});

	glm::vec3 startPosition = glm::vec3(-800.0f, 50.0f, -35.0f);
	for (int i = 0; i < 5; i++)
	{
		GameObject pointLightObject = GetComponentSystem().CreateGameObject("PointLight");
		PointLightComponent* pointLight = pointLightObject.AddComponent<PointLightComponent>();
		
		const auto& transform = pointLightObject.GetTransform();
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

void World::SaveToFile(const std::filesystem::path& inPath)
{
	BinarySerializer writer(inPath, BinarySerializer::Mode::Write);
	// writer.WriteClass(this, Engine::GetReflectionSystem().GetClass(this));
	writer.SerializeType(GetComponentSystem());
	writer.Close();
}

void World::LoadFromFile(const std::filesystem::path& inPath)
{
	check(false);
}

bool World::Raycast(const glm::vec3& inOrigin, const glm::vec3& inDirection, RaycastHit& outHit, const float inMaxDistance, const TagMask inExcludedTags, bool inIgnoreTriggers)
{
	check(length(inDirection) != 0);
	
	if (inIgnoreTriggers || inExcludedTags != 0)
	{
		List<RaycastHit> hits;
		bool result = RaycastAll(inOrigin, inDirection, hits, inMaxDistance, inIgnoreTriggers, inExcludedTags);
		
		if (!result)
			return false;

		std::sort(hits.begin(), hits.end(), [](const RaycastHit& lhs, const RaycastHit& rhs)
		{
			return lhs.myHitDistance < rhs.myHitDistance;
		});

		outHit = hits[0];
		return true;
	}

	PhysicsSystem& physicsSystem = GetWorldSystem<PhysicsSystem>();
	physx::PxRaycastBuffer hitBuffer;
	physx::PxVec3 origin = {inOrigin.x, inOrigin.y, inOrigin.z};
	physx::PxVec3 direction = {inDirection.x, inDirection.y, inDirection.z};
	direction.normalize();
	bool result = physicsSystem.GetScene()->raycast(origin, direction, inMaxDistance, hitBuffer);

	if (!result)
		return false;

	const physx::PxRaycastHit& touch = hitBuffer.block;
	
	outHit.myHitDistance = touch.distance;
	outHit.myHitPosition = { touch.position.x, touch.position.y, touch.position.z };
	outHit.myHitNormal = { touch.normal.x, touch.normal.y, touch.normal.z };
	outHit.myHitGameObject = (GameObject*)touch.actor->userData;
	return true;
}

bool World::RaycastAll(const glm::vec3& inOrigin, const glm::vec3& inDirection, List<RaycastHit>& outHits, const float inMaxDistance, const TagMask inExcludedTags, bool inIgnoreTriggers)
{
	check(length(inDirection) != 0);
	
	PhysicsSystem& physicsSystem = GetWorldSystem<PhysicsSystem>();

	physx::PxRaycastBuffer hitBuffer;
	physx::PxRaycastHit touches[32];
	hitBuffer.touches = touches;
	hitBuffer.maxNbTouches = 32;

	physx::PxQueryFilterData filterData;
	filterData.flags |= physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::ePREFILTER | physx::PxQueryFlag::eNO_BLOCK;

	physx::PxVec3 origin = {inOrigin.x, inOrigin.y, inOrigin.z};
	physx::PxVec3 direction = {inDirection.x, inDirection.y, inDirection.z};
	direction.normalize();
	
	bool result = physicsSystem.GetScene()->raycast(origin, direction, inMaxDistance, hitBuffer, physx::PxHitFlag::eDEFAULT, filterData);
	if (!result)
		return false;

	for (uint i = 0; i < hitBuffer.nbTouches; i++)
	{
		physx::PxRaycastHit touch = hitBuffer.getTouch(i);

		if (inIgnoreTriggers)
		{
			if (touch.shape->getFlags().isSet(physx::PxShapeFlag::eTRIGGER_SHAPE))
				continue;
		}

		GameObject* gameObject = (GameObject*)touch.actor->userData;

		if(gameObject->HasAnyTag(inExcludedTags))
			continue;
		
		RaycastHit& outHit = outHits.Emplace();
		outHit.myHitDistance = touch.distance;
		outHit.myHitNormal = { touch.normal.x, touch.normal.y, touch.normal.z };
		outHit.myHitPosition = { touch.position.x, touch.position.y, touch.position.z };
		outHit.myHitGameObject = gameObject;
	}

	return !outHits.IsEmpty();
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

void World::CreateWorldSystems()
{
	mySystemManager->AddSystem<ComponentSystem>(this);
	mySystemManager->AddSystem<LandscapeSystem>(this);
	
	// Keep the physics system last so we make sure we add all object updates before we do any physics calculations.
	mySystemManager->AddSystem<PhysicsSystem>(this);

	mySystemManager->InitAllSystems();
}
