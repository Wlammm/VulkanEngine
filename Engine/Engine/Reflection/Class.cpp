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

const List<Method>& Class::GetMethods() const
{
    return myMethods;
}

unsigned int Class::GetSize() const
{
    return myByteSize;
}

bool Class::IsCopyable() const
{
    return myIsCopyable;
}

List<Field> Class::GetFieldsWithMetadata(const std::string& inMetadata) const
{
    List<Field> fields;

    for (const Field& field : myFields)
    {
        if (field.HasMetadata(inMetadata))
            fields.Add(field);
    }
    return fields;
}

List<const Method*> Class::GetMethodsWithMetadata(const std::string& inMetadata) const
{
    List<const Method*> methods;
    for (const Method& method : myMethods)
    {
        if (method.HasMetadata(inMetadata))
        {
            methods.Add(&method);
        }
    }
    return methods;
}

const Method* Class::GetMethod(const std::string& inMethodName) const
{
    for (const Method& method : myMethods)
    {
        if (method.GetMethodName() == inMethodName)
            return &method;
    }
    return nullptr;
}

Field& Class::AddField(const Field& inField)
{
    return myFields.Emplace(inField);
}

Method& Class::AddMethod(const Method& inMethod)
{
    return myMethods.Emplace(inMethod);
}

void Class::AddBaseClass(Class* inBaseClass)
{
    if (!inBaseClass)
        return;
        
    myBaseClasses.Add(inBaseClass);
    inBaseClass->myDerivedClasses.Add(this);
}

Class::Class(const std::string& inClassName, const std::string& inFullName, const unsigned int inSize, const bool inIsCopyable,  const Delegate<void*()>& inFactoryFunction)
    : myClassName(inClassName), myFullName(inFullName), myByteSize(inSize), myIsCopyable(inIsCopyable), myFactoryFunction(inFactoryFunction)
{ }
