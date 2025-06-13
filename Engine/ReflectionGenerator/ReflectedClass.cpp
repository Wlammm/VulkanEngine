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

nlohmann::json ReflectedClass::Save() const
{
    nlohmann::json json;
    json["className"] = myClassName;

    for (const ReflectedField& field : myFields)
    {
        json["fields"].push_back(field.Save());
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
}

void ReflectedClass::AddField(const ReflectedField& inField)
{
    myFields.emplace_back(inField);
}
