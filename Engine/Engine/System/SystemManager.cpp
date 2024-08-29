#include "EnginePch.h"
#include "SystemManager.h"

#include "System.h"

SystemManager::~SystemManager()
{
    for(System* system : mySystems)
    {
        del(system);
    }
    mySystems.Clear();
}

void SystemManager::InitAllSystems()
{
    for(System* system : mySystems)
    {
        system->Init();
    }
}