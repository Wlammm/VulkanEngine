#include "EnginePch.h"
#include "Class.h"

#include "ReflectionSystem.h"
#include "Engine/Engine.h"

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

bool Class::IsTemplateSpecialization() const
{
    return myIsTemplateSpecialization;
}

const List<const Class*>& Class::GetTemplateArgumentTypes() const
{
    return myTemplateArguments;
}

std::string Class::GetNonTemplatedName() const
{
    return myNonTemplateName;
}

std::string Class::GetClassNameWithoutForwardDeclares(const std::string& inClassName)
{
    std::istringstream iss(inClassName);
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

Class::Class(const std::string& inClassName, const std::string& inFullName, const unsigned int inSize, const bool inIsCopyable,  const Delegate<void*()>& inFactoryFunction)
    : myClassName(inClassName), myFullName(inFullName), myByteSize(inSize), myIsCopyable(inIsCopyable), myFactoryFunction(inFactoryFunction)
{
    if (inClassName.contains('<') && inClassName.contains('>'))
    {
        myIsTemplateSpecialization = true;
        myNonTemplateName = inClassName.substr(0, inClassName.find_first_of('<'));
        
        const List<std::string> templateArgClassNames = SplitTemplateArgs(inFullName);
        for (const std::string& arg : templateArgClassNames)
        {
            const std::string sanitizedClassName = GetClassNameWithoutForwardDeclares(arg);
            const Class* templateArgClass = Engine::GetReflectionSystem().GetClassByName(sanitizedClassName);

            // TODO: Some template arguments doesnt have a class implementation so it will be nullptr. Not all template arguments are recognized.
            if (!templateArgClass)
                continue;
            
            // check(templateArgClass);
            myTemplateArguments.Add(templateArgClass);
        }
    }
    
}
