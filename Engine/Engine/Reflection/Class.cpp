#include "EnginePch.h"
#include "Class.h"

const std::string& Class::GetName() const
{
    return myClassName;
}

const std::string& Class::GetFullName() const
{
    return myFullName;
}

const List<const Class*>& Class::GetBaseClasses() const
{
    return myBaseClasses;
}

const List<const Class*>& Class::GetDerivedClasses() const
{
    return myDerivedClasses;
}

const List<Field>& Class::GetFields() const
{
    return myFields;
}

Class::Class(const std::string& inClassName, const std::string& inFullName, Delegate<void*()> inFactoryFunction)
    : myClassName(inClassName), myFullName(inFullName)
{ }
