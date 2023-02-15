#include "EnginePch.h"
#include "World.h"

World::World()
{

}

World::~World()
{
	
}

const entt::registry& World::GetRegistry() const
{
	return myRegistry;
}