#pragma once
#include "Field.h"
#include "Engine/Containers/List.hpp"

class ReflectedClass 
{
public:
    const std::string& GetName() const;
    const std::string& GetFullName() const;

    const List<const ReflectedClass*>& GetBaseClasses() const;
    const List<const ReflectedClass*>& GetDerivedClasses() const;

    const List<Field>& GetFields() const;

private:
    friend class ReflectionSystem;
    friend class GeneratedReflectionData;
    void AddField(const Field& inField)
    {
        myFields.Add(inField);
    }

    void AddBaseClass(ReflectedClass* inBaseClass)
    {
        myBaseClasses.Add(inBaseClass);
        inBaseClass->myDerivedClasses.Add(this);
    }
    
    ReflectedClass() = delete;
    ReflectedClass(const std::string& inClassName, const std::string& inFullName);
    
    // This is the full name which matches what you get when using typeid(ClassType).name()
    std::string myFullName = "";
    
    // This is a prettified class name. It'll return just the ClassName.
    std::string myClassName = "";

    List<const ReflectedClass*> myBaseClasses{};
    List<const ReflectedClass*> myDerivedClasses{};
    
    List<Field> myFields{};
};
