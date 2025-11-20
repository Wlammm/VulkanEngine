#include "EnginePch.h"
#include "Type.h"

#include "ReflectionSystem.h"
#include "Engine/Engine.h"

const std::string& Type::GetName() const
{
    return myTypeName;
}

const std::string& Type::GetFullName() const
{
    return myFullName;
}

const List<const Type*>& Type::GetBaseTypes() const
{
    return myBaseTypes;
}

const List<const Type*>& Type::GetDerivedTypes() const
{
    return myDerivedTypes;
}

const List<Field>& Type::GetFields() const
{
    return myFields;
}

const List<Method>& Type::GetMethods() const
{
    return myMethods;
}

const Field* Type::FindField(const std::string& inFieldName) const
{
    for (const Field& field : myFields)
    {
        if (field.GetName() == inFieldName)
            return &field;
    }
    return nullptr;
}

unsigned int Type::GetSize() const
{
    return myByteSize;
}

const Field* Type::FindFieldByName(const std::string& inFieldName) const
{
    for (Field& field : myFields)
    {
        if (field.GetName() == inFieldName)
            return &field;
    }
    return nullptr;
}

const Field* Type::FindFieldByNameRecursive(const std::string& inFieldName) const
{
    if (const Field* field = FindFieldByName(inFieldName))
        return field;

    for (const Type* base : myBaseTypes)
    {
        if (const Field* field = base->FindFieldByNameRecursive(inFieldName))
            return field;
    }
    
    return nullptr;
}

bool Type::IsCopyable() const
{
    return myIsCopyable;
}

List<Field> Type::GetFieldsWithMetadata(const std::string& inMetadata) const
{
    List<Field> fields;

    for (const Field& field : myFields)
    {
        if (field.HasMetadata(inMetadata))
            fields.Add(field);
    }
    return fields;
}

List<Field> Type::GetFieldsWithMetadataRecursive(const std::string& inMetadata) const
{
    List<Field> fields;

    fields.AddRange(GetFieldsWithMetadata(inMetadata));
    for (const Type* type : myBaseTypes)
    {
        fields.AddRange(type->GetFieldsWithMetadataRecursive(inMetadata));
    }
    return fields;
}

List<const Method*> Type::GetMethodsWithMetadata(const std::string& inMetadata) const
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

const Method* Type::GetMethod(const std::string& inMethodName) const
{
    for (const Method& method : myMethods)
    {
        if (method.GetMethodName() == inMethodName)
            return &method;
    }
    return nullptr;
}

bool Type::IsTemplateSpecialization() const
{
    return !myTemplateArguments.IsEmpty();
}

const List<TypeTemplateArgument>& Type::GetTemplateArguments() const
{
    return myTemplateArguments;
}

std::string Type::GetTypeNameWithoutForwardDeclares(const std::string& inTypeName)
{
    std::istringstream iss(inTypeName);
    std::string word;

    // Read first word
    iss >> word;

    // Check if it's "class" or "struct"
    if (word == "class" || word == "struct") {
        // Get the next word which should be the actual name
        iss >> word;
    }

    return word;
}

void Type::PlacementNew(void* destination) const
{
    check(myPlacementFactoryFunction.IsValid() && "No placement factory function bound!");
    myPlacementFactoryFunction(destination);
}

Field& Type::AddField(const Field& inField)
{
    return myFields.Emplace(inField);
}

Method& Type::AddMethod(const Method& inMethod)
{
    return myMethods.Emplace(inMethod);
}

void Type::AddBaseType(Type* inBaseType)
{
    if (!inBaseType)
        return;

    myBaseTypes.Add(inBaseType);
    inBaseType->myDerivedTypes.Add(this);
}

List<std::string> SplitTemplateArgs(const std::string& input)
{
    List<std::string> result;
    int angleDepth = 0;
    std::string current;

    for (size_t i = 0; i < input.size(); ++i)
    {
        char c = input[i];

        if (c == '<')
        {
            angleDepth++;
            current += c;
        }
        else if (c == '>')
        {
            angleDepth--;
            current += c;
        }
        else if (c == ',' && angleDepth == 0)
        {
            if (!current.empty())
            {
                size_t start = current.find_first_not_of(" \t\n\r");
                size_t end = current.find_last_not_of(" \t\n\r");
                if (start != std::string::npos && end != std::string::npos)
                    result.Add(current.substr(start, end - start + 1));
                else
                    result.Add("");
                current.clear();
            }
        } else
        {
            current += c;
        }
    }

    if (!current.empty())
    {
        size_t start = current.find_first_not_of(" \t\n\r");
        size_t end = current.find_last_not_of(" \t\n\r");
        if (start != std::string::npos && end != std::string::npos)
            result.Add(current.substr(start, end - start + 1));
        else
            result.Add("");
    }

    return result;
}

Type::Type(const std::string& inTypeName, const std::string& inFullName, const unsigned int inSize, const bool inIsCopyable,  const Delegate<void*()>& inFactoryFunction, const Delegate<void(void*)>& inPlacementFactoryFunction, const Delegate<void(IUniquePtr*)>& inUniquePtrFactoryFunction)
    : myTypeName(inTypeName), myFullName(inFullName), myByteSize(inSize), myIsCopyable(inIsCopyable), myFactoryFunction(inFactoryFunction), myPlacementFactoryFunction(inPlacementFactoryFunction), myUniquePtrFactoryFunction(inUniquePtrFactoryFunction)
{
}
