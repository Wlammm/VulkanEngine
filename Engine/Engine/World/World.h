#pragma once
#include "Engine/Core/AutoInit.h"
#include "Engine/System/SystemManager.hpp"
#include "Engine/System/WorldSystem.h"

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

	//bool Raycast(const glm::vec3& inOrigin, const glm::vec3& inDirection, )
	
	class AssetRegistry& GetAssetRegistry() const;

	DirectionalLightComponent* GetDirectionalLight() const;

	ComponentSystem& GetComponentSystem() const;
	
	void ToggleCactus();

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

	class GameObject* myCactus = nullptr;
};