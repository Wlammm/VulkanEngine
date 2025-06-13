#include "EnginePch.h"
#include "ReflectionSystem.h"

void ReflectionSystem::AddClass(const std::string& inClassName, const std::string& inFullName)
{
    myClasses.Add(Class(inClassName, inFullName));
}
