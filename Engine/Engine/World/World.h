#pragma once
#include "Engine/ComponentSystem/GameObject.h"
#include "Engine/Core/AutoInit.h"
#include "Engine/System/SystemManager.hpp"
#include "Engine/System/WorldSystem.h"
#include "Engine/Physics/PhysicsQueryStructs.h"

class DirectionalLightComponent;
class ComponentSystem;
class ECSRegistry;
struct DirectionalLight;

class World : public AutoInit
{
public:
	World();
	~World();

	void Init() override final;
	void Update();
	void Destroy();

	void SaveToFile(const std::filesystem::path& inPath);
	void LoadFromFile(const std::filesystem::path& inPath);

	bool Raycast(const glm::vec3& inOrigin, const glm::vec3& inDirection, RaycastHit& outHit, const float inMaxDistance = FLOAT_MAX, const TagMask inExcludedTags = 0, bool inIgnoreTriggers = true);
	bool RaycastAll(const glm::vec3& inOrigin, const glm::vec3& inDirection, List<RaycastHit>& outHits, const float inMaxDistance = FLOAT_MAX, const TagMask inExcludedTags = 0, bool inIgnoreTriggers = true);
	
	class AssetRegistry& GetAssetRegistry() const;

	DirectionalLightComponent* GetDirectionalLight() const;

	ComponentSystem& GetComponentSystem() const;
	
	template<typename SystemType>
	SystemType& GetWorldSystem() const
	{
		return mySystemManager->GetSystem<SystemType>();
	}

private:
	void CreateWorldSystems();
	
private:
	class AssetRegistry* myAssetRegistry = nullptr;

	SystemManager<WorldSystem>* mySystemManager = nullptr;
};