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

const List<Class*>& ReflectionSystem::GetAllClasses() const
{
    return myClasses;
}
