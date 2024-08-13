#pragma once
#include "Core/AutoInit.h"

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

	class AssetRegistry& GetAssetRegistry() const;

	DirectionalLightComponent* GetDirectionalLight() const;

	ComponentSystem& GetComponentSystem() const;
	
	
private:
	class AssetRegistry* myAssetRegistry = nullptr;

	ComponentSystem* myComponentSystem = nullptr;
};