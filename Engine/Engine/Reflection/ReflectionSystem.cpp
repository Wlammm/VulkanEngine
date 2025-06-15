#include "EnginePch.h"
#include "ReflectionSystem.h"

ReflectionSystem::~ReflectionSystem()
{
    for (Class* entry : myClasses)
    {
        del(entry);
    }
    myClasses.Clear();
}
