#include "EnginePch.h"
#include "World.h"

#include <PxRigidActor.h>
#include <PxScene.h>

#include "Engine/Engine.h"
#include "Engine/Components/DirectionalLightComponent.h"
#include "Engine/Components/PointLightComponent.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/ComponentSystem/Actor.h"
#include "Engine/ComponentSystem/GameObjectTag.hpp"
#include "Engine/ComponentSystem/Actors/DirectionalLightActor.h"
#include "Engine/ComponentSystem/Actors/LandscapeActor.h"
#include "Engine/ComponentSystem/Actors/PhysicsCubeActor.h"
#include "Engine/ComponentSystem/Actors/PointLightActor.h"
#include "Engine/ComponentSystem/Actors/StaticMeshActor.h"
#include "Engine/Physics/PhysicsSystem.h"
#include "Engine/Reflection/Type.h"
#include "Engine/Serialization/BinarySerializer.h"
#include "Engine/Systems/LandscapeSystem.h"

World::World()
{
	mySystemManager = MakeUnique<SystemManager<WorldSystem>>();
	CreateWorldSystems();

	myCachedDirectionalLightActor = SpawnActor<DirectionalLightActor>("DirectionalLight");
	myCachedDirectionalLightActor->GetTransform().SetRotationDeg(321, 314, -50);
	myCachedDirectionalLightActor->GetDirectionalLightComponent().SetColor({1, 168/255.0, 120/255.0, 1});
}

World::~World()
{
	mySystemManager.Reset();
}

void World::PostPropertiesSerialized()
{
	Asset::PostPropertiesSerialized();
	
	for (const UniquePtr<Actor>& actor : myActors)
	{
		actor->myWorld = this;
		actor->RegisterComponents();
	}

	for (const UniquePtr<Actor>& actor : myActors)
	{
		actor->DoOnCreate();
		
		if (actor->IsTransientActor())
			RemoveActor(actor.Get());
	}
}

void World::DoTick()
{
	Tick();

	TickActorDeletes();
}

void World::Destroy()
{
	
}

void World::TickPhysics()
{
	for (const UniquePtr<Actor>& actor : myActors)
	{
		actor->DoTickPhysics();
	}
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
	outHit.myHitActor = (Actor*)touch.actor->userData;
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

		Actor* actor = (Actor*)touch.actor->userData;

		if(actor->HasAnyTag(inExcludedTags))
			continue;
		
		RaycastHit& outHit = outHits.Emplace();
		outHit.myHitDistance = touch.distance;
		outHit.myHitNormal = { touch.normal.x, touch.normal.y, touch.normal.z };
		outHit.myHitPosition = { touch.position.x, touch.position.y, touch.position.z };
		outHit.myHitActor = actor;
	}

	return !outHits.IsEmpty();
}

Actor* World::SpawnActor(const Type* inActorType, const std::string& inName)
{
	myActors.Emplace();
	IUniquePtr* actorPtr = &myActors.Last();
	inActorType->CreateUniquePtr(actorPtr);
	Actor* actor = myActors.Last().Get();
	InitActor(actor, inName);
	return actor;
}

void World::RemoveActor(Actor* inActor)
{
	myActorsToDelete.Emplace(inActor);
}

void World::RemoveAllActors()
{
	for (const UniquePtr<Actor>& actor : myActors)
	{
		RemoveActor(actor.Get());
	}
}

const List<UniquePtr<Actor>>& World::GetAllActors() const
{
	return myActors;
}

void World::OnTriggerEnter(Actor* inFirst, Actor* inOther)
{
	inFirst->DoOnTriggerEnter(inOther);
	inOther->DoOnTriggerEnter(inFirst);
}

void World::OnTrigger(Actor* inFirst, Actor* inOther)
{
	inFirst->DoOnTrigger(inOther);
	inOther->DoOnTrigger(inFirst);
}

void World::OnTriggerExit(Actor* inFirst, Actor* inOther)
{
	inFirst->DoOnTriggerExit(inOther);
	inOther->DoOnTriggerExit(inFirst);
}

void World::OnCollisionEnter(Actor* inFirst, Actor* inOther)
{
	inFirst->DoOnCollisionEnter(inOther);
	inOther->DoOnCollisionEnter(inFirst);
}

void World::OnCollision(Actor* inFirst, Actor* inOther)
{
	inFirst->DoOnCollision(inOther);
	inOther->DoOnCollision(inFirst);
}

void World::OnCollisionExit(Actor* inFirst, Actor* inOther)
{
	inFirst->DoOnCollisionExit(inOther);
	inOther->DoOnCollisionExit(inFirst);
}

DirectionalLightComponent* World::GetDirectionalLight() const
{
	if (!myCachedDirectionalLightActor)
	{
		myCachedDirectionalLightActor = FindActorOfType<DirectionalLightActor>();
		if (!myCachedDirectionalLightActor)
			return nullptr;
	}
	
	return &myCachedDirectionalLightActor->GetDirectionalLightComponent();
}

void World::SetMainCamera(CameraComponent* inCamera)
{
	myMainCamera = inCamera;
}

CameraComponent* World::GetMainCamera() const
{
	return myMainCamera;
}

void World::CreateWorldSystems()
{
	mySystemManager->AddSystem<LandscapeSystem>(this);
	
	// Keep the physics system last so we make sure we add all object updates before we do any physics calculations.
	mySystemManager->AddSystem<PhysicsSystem>(this);

	mySystemManager->InitAllSystems();
}

void World::TickActorDeletes()
{
	for (Actor* actor : myActorsToDelete)
	{
		for (int i = 0; i < myActors.size(); ++i)
		{
			if (myActors[i] == actor)
			{
				myActors[i]->DoOnDestroy();
				myActors.RemoveIndex(i);
				break;
			}
		}
	}
	myActorsToDelete.Clear();
}

void World::InitActor(Actor* inActor, const std::string& inName)
{
	inActor->myWorld = this;
	inActor->SetName(inName);
	inActor->RegisterComponents();
	inActor->DoOnCreate();
}
