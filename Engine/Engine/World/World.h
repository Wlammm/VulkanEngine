#pragma once
#include "Engine/Core/AutoInit.h"

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

	class AssetRegistry& GetAssetRegistry() const;

	DirectionalLightComponent* GetDirectionalLight() const;

	ComponentSystem& GetComponentSystem() const;
	
	void ToggleCactus();
	
private:
	class AssetRegistry* myAssetRegistry = nullptr;

	ComponentSystem* myComponentSystem = nullptr;

	class GameObject* myCactus = nullptr;
	
};