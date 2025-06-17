#pragma once
#include "Field.h"
#include "Method.h"
#include "Engine/Containers/List.hpp"
#include "Engine/Delegates/Delegate.hpp"

class Class 
{
public:
    const std::string& GetName() const;
    const std::string& GetFullName() const;

    const List<const Class*>& GetBaseClasses() const;
    const List<const Class*>& GetDerivedClasses() const;

    const List<Field>& GetFields() const;
    const List<Method>& GetMethods() const;

    List<Field> GetFieldsWithMetadata(const std::string& inMetadata) const;
    List<const Method*> GetMethodsWithMetadata(const std::string& inMetadata) const;
    
    const Method* GetMethod(const std::string& inMethodName) const;
     
    template<typename ClassType>
    bool IsA() const
    {
        const std::string fullName = typeid(ClassType).name();
        if (fullName == myFullName)
            return true;

        for (const Class* baseClass : myBaseClasses)
        {
            if (baseClass->GetFullName() == fullName)
                return true;
        }
        return false;
    }
    
    // TODO: Support sending constructor arguments here. This can be done but we need to parse the arguments via the reflection generator first.
    template<typename ClassType>
    ClassType* CreateInstance() const
    {
        check(IsA<ClassType>());
        void* instance = myFactoryFunction();
        return static_cast<ClassType*>(instance);
    }

private:
    friend class ReflectionSystem;
    friend class GeneratedReflectionData;
    
    Field& AddField(const Field& inField);
    Method& AddMethod(const Method& inMethod);
    void AddBaseClass(Class* inBaseClass);
    
    Class() = delete;
    Class(const std::string& inClassName, const std::string& inFullName, const Delegate<void*()>& inFactoryFunction);
    
    // This is the full name which matches what you get when using typeid(ClassType).name()
    std::string myFullName = "";
    
    // This is a prettified class name. It'll return just the ClassName.
    std::string myClassName = "";

    // This is a callback that creates an instance of this class type.
    Delegate<void*()> myFactoryFunction;
    
    List<const Class*> myBaseClasses{};
    List<const Class*> myDerivedClasses{};
    
    List<Field> myFields{};
    List<Method> myMethods{};
};
