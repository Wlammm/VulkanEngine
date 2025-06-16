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
}

void ReflectedClass::AddField(const ReflectedField& inField)
{
    myFields.emplace_back(inField);
}

void ReflectedClass::AddMethod(const ReflectedMethod& inMethod)
{
    myMethods.push_back(inMethod);
}
