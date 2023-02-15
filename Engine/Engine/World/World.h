#pragma once

class World 
{
public:
	World();
	~World();

	const entt::registry& GetRegistry() const;

private:
	entt::registry myRegistry{};
};