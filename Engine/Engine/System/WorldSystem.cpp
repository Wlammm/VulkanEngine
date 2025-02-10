#include "EnginePch.h"
#include "WorldSystem.h"

WorldSystem::WorldSystem(World* inWorld)
{
    myWorld = inWorld;
}

World* WorldSystem::GetWorld() const
{
    return myWorld;
}
