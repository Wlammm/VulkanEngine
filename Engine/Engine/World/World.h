#pragma once

#include <entt/entt.hpp>

class World 
{
public:
	World();
	~World();

	const entt::registry& GetRegistry() const;

private:
	entt::registry myRegistry{};
};