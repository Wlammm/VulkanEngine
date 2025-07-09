#pragma once
#include "Field.h"
#include "Method.h"
#include "Engine/Containers/List.hpp"
#include "Engine/Delegates/Delegate.hpp"

struct TypeTemplateArgument
{
    const Type* myType;
    const bool myIsPointer;
    const bool myIsReference;
};

class Type 
{
public:
    const std::string& GetName() const;
    const std::string& GetFullName() const;

    const List<const Type*>& GetBaseTypes() const;
    const List<const Type*>& GetDerivedTypes() const;

    const List<Field>& GetFields() const;
    const List<Method>& GetMethods() const;
    const Field* FindField(const std::string& inFieldName) const; 

    unsigned int GetSize() const;

    const Field* FindFieldByName(const std::string& inFieldName) const;
    
    bool IsCopyable() const;

    List<Field> GetFieldsWithMetadata(const std::string& inMetadata) const;
    List<const Method*> GetMethodsWithMetadata(const std::string& inMetadata) const;
    
    const Method* GetMethod(const std::string& inMethodName) const;

    bool IsTemplateSpecialization() const;
    const List<TypeTemplateArgument>& GetTemplateArguments() const;

    static std::string GetTypeNameWithoutForwardDeclares(const std::string& inTypeName);
    
    template<typename ClassType>
    bool IsA() const
    {
        const std::string fullName = typeid(ClassType).name();
        if (fullName == myFullName)
            return true;

        for (const Type* baseType : myBaseTypes)
        {
            if (baseType->GetFullName() == fullName)
                return true;

            if (baseType->IsA<ClassType>())
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

    void PlacementNew(void* destination) const;

    void AddTemplateArgument(const Type* inTemplateArgumentType, const bool inIsPointer, const bool inIsReference)
    {
        myTemplateArguments.Add({ inTemplateArgumentType, inIsPointer, inIsReference });
    }

private:
    friend class ReflectionSystem;
    friend class GeneratedReflectionData;
    
    Field& AddField(const Field& inField);
    Method& AddMethod(const Method& inMethod);
    void AddBaseType(Type* inBaseType);
    
    Type() = delete;
    Type(const std::string& inTypeName, const std::string& inFullName, const unsigned int inSize, const bool inIsCopyable,  const Delegate<void*()>& inFactoryFunction, const Delegate<void(void*)>& inPlacementFactoryFunction);
    
    // This is the full name which matches what you get when using typeid(ClassType).name()
    std::string myFullName = "";
    
    // This is a prettified class name. It'll return just the TypeName.
    std::string myTypeName = "";

    unsigned int myByteSize = 0;

    bool myIsCopyable = false;
    
    // This is a callback that creates an instance of this type.
    Delegate<void*()> myFactoryFunction;
    Delegate<void(void* destination)> myPlacementFactoryFunction;
    
    List<const Type*> myBaseTypes{};
    List<const Type*> myDerivedTypes{};

    List<Field> myFields{};
    List<Method> myMethods{};

    List<TypeTemplateArgument> myTemplateArguments{};
};
