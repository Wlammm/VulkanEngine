#pragma once

class World 
{
public:
	World();
	~World();

	entt::registry& GetRegistry();

private:
	entt::registry myRegistry{};
};