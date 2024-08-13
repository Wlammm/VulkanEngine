#include "EnginePch.h"
#include "SystemManager.h"

#include "System.h"

SystemManager::~SystemManager()
{
    for(System* subsystem : mySubsystems)
    {
        del(subsystem);
    }
    mySubsystems.Clear();
}