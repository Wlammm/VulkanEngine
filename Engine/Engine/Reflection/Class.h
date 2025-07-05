#pragma once
#include "Field.h"
#include "Method.h"
#include "Engine/Containers/List.hpp"
#include "Engine/Delegates/Delegate.hpp"

struct ClassTemplateArgument
{
    const Class* myType;
    const bool myIsPointer;
    const bool myIsReference;
};

class Class 
{
public:
    const std::string& GetName() const;
    const std::string& GetFullName() const;

    const List<const Class*>& GetBaseClasses() const;
    const List<const Class*>& GetDerivedClasses() const;

    const List<Field>& GetFields() const;
    const List<Method>& GetMethods() const;

    unsigned int GetSize() const;

    const Field* FindFieldByName(const std::string& inFieldName) const;
    
    bool IsCopyable() const;

    List<Field> GetFieldsWithMetadata(const std::string& inMetadata) const;
    List<const Method*> GetMethodsWithMetadata(const std::string& inMetadata) const;
    
    const Method* GetMethod(const std::string& inMethodName) const;

    bool IsTemplateSpecialization() const;
    const List<ClassTemplateArgument>& GetTemplateArguments() const;

    static std::string GetClassNameWithoutForwardDeclares(const std::string& inClassName);
    
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

            if (baseClass->IsA<ClassType>())
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

    void AddTemplateArgument(const Class* inTemplateArgumentClass, const bool inIsPointer, const bool inIsReference)
    {
        myTemplateArguments.Add({ inTemplateArgumentClass, inIsPointer, inIsReference });
    }

private:
    friend class ReflectionSystem;
    friend class GeneratedReflectionData;
    
    Field& AddField(const Field& inField);
    Method& AddMethod(const Method& inMethod);
    void AddBaseClass(Class* inBaseClass);
    
    Class() = delete;
    Class(const std::string& inClassName, const std::string& inFullName, const unsigned int inSize, const bool inIsCopyable,  const Delegate<void*()>& inFactoryFunction);
    
    // This is the full name which matches what you get when using typeid(ClassType).name()
    std::string myFullName = "";
    
    // This is a prettified class name. It'll return just the ClassName.
    std::string myClassName = "";

    unsigned int myByteSize = 0;

    bool myIsCopyable = false;
    
    // This is a callback that creates an instance of this class type.
    Delegate<void*()> myFactoryFunction;
    
    List<const Class*> myBaseClasses{};
    List<const Class*> myDerivedClasses{};

    List<Field> myFields{};
    List<Method> myMethods{};

    List<ClassTemplateArgument> myTemplateArguments{};
};
