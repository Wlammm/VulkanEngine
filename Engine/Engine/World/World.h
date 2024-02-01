#pragma once
#include "Core/AutoInit.h"

struct DirectionalLight;

class World : public AutoInit
{
public:
	World();
	~World();

	void Init() override final;

	entt::registry& GetRegistry();
	class AssetRegistry& GetAssetRegistry();

	const DirectionalLight& GetDirectionalLight() const;

private:
	entt::registry myRegistry{};
	class AssetRegistry* myAssetRegistry = nullptr;

	const DirectionalLight* myDirectionalLight = nullptr;
};