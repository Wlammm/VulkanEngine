#include "EnginePch.h"
#include "ReflectedClass.h"

const std::string& ReflectedClass::GetName() const
{
    return myClassName;
}

const std::string& ReflectedClass::GetFullName() const
{
    return myFullName;
}

const List<const ReflectedClass*>& ReflectedClass::GetBaseClasses() const
{
    return myBaseClasses;
}

const List<const ReflectedClass*>& ReflectedClass::GetDerivedClasses() const
{
    return myDerivedClasses;
}

const List<Field>& ReflectedClass::GetFields() const
{
    return myFields;
}

ReflectedClass::ReflectedClass(const std::string& inClassName, const std::string& inFullName)
    : myClassName(inClassName), myFullName(inFullName)
{ }
