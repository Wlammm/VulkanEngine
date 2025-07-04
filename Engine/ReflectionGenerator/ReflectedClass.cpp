#include "ReflectedClass.h"

#include "../ImGui/imgui_internal.h"


ReflectedClass::ReflectedClass()
{
}

ReflectedClass::ReflectedClass(const std::string& inClassName)
{
    myClassName = inClassName;
}

const std::string& ReflectedClass::GetClassName() const
{
    return myClassName;
}

const std::vector<ReflectedField>& ReflectedClass::GetFields() const
{
    return myFields;
}

const std::vector<std::string>& ReflectedClass::GetBaseTypeNames() const
{
    return myBaseTypeNames;
}

const std::vector<ReflectedMethod>& ReflectedClass::GetMethods() const
{
    return myMethods;
}

void ReflectedClass::AddBaseClass(const std::string& inBaseTypeName)
{
    myBaseTypeNames.push_back(inBaseTypeName);
}

nlohmann::json ReflectedClass::Save() const
{
    nlohmann::json json;
    json["className"] = myClassName;

    for (const ReflectedField& field : myFields)
    {
        json["fields"].push_back(field.Save());
    }

    for (const ReflectedMethod& method : myMethods)
    {
        json["methods"].push_back(method.Save());
    }

    for (const std::string& baseTypeName : myBaseTypeNames)
    {
        json["baseTypeNames"].push_back(baseTypeName);
    }

    for (const ReflectedClassTemplateArgument& templateArg : myTemplateArguments)
    {
        nlohmann::json arg;
        arg["typeName"] = templateArg.myTypeName;
        arg["isPointer"] = templateArg.myIsPointer;
        arg["isReference"] = templateArg.myIsReference;
        
        json["templateArguments"].push_back(arg);
    }
    
    return json;
}

void ReflectedClass::Load(const nlohmann::json& inJson)
{
    myClassName = inJson["className"].get<std::string>();

    if (inJson.contains("fields"))
    {
        for (const nlohmann::json& field : inJson["fields"])
        {
            myFields.emplace_back();
            myFields.back().Load(field);
        }
    }

    if (inJson.contains("methods"))
    {
        for (const nlohmann::json& method : inJson["methods"])
        {
            myMethods.emplace_back();
            myMethods.back().Load(method);
        }
    }

    if (inJson.contains("baseTypeNames"))
    {
        myBaseTypeNames = inJson["baseTypeNames"].get<std::vector<std::string>>();
    }

    if (inJson.contains("templateArguments"))
    {
        for (nlohmann::json json : inJson["templateArguments"])
        {
            ReflectedClassTemplateArgument arg;
            arg.myTypeName = json["typeName"].get<std::string>();
            arg.myIsPointer = json["isPointer"].get<bool>();
            arg.myIsReference = json["isReference"].get<bool>();
            myTemplateArguments.emplace_back(arg);
        }
    }
}

ReflectedField& ReflectedClass::AddField(const ReflectedField& inField)
{
    return myFields.emplace_back(inField);
}

void ReflectedClass::AddMethod(const ReflectedMethod& inMethod)
{
    myMethods.push_back(inMethod);
}

void ReflectedClass::AddTemplateArgument(const int inIndex, const std::string& inTemplateArgument, const bool inIsPointer, const bool inIsReference)
{
    // If we already have the template argument just verify it is the same as we want it now and return.
    if (myTemplateArguments.size() > inIndex)
    {
        assert(myTemplateArguments[inIndex].myTypeName == inTemplateArgument);
        return;
    }
    myTemplateArguments.push_back({ inTemplateArgument, inIsPointer, inIsReference });   
}

const std::vector<ReflectedClassTemplateArgument>& ReflectedClass::GetTemplateArguments() const
{
    return myTemplateArguments;
}
