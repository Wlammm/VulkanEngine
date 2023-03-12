#pragma once

class World 
{
public:
	World();
	~World();

	entt::registry& GetRegistry();
	class AssetRegistry& GetAssetRegistry();

private:
	entt::registry myRegistry{};
	class AssetRegistry* myAssetRegistry = nullptr;
};