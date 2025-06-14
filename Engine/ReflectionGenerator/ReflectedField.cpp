#include "ReflectedField.h"

ReflectedField::ReflectedField()
{
}

ReflectedField::ReflectedField(const std::string& inFieldName, const std::string& inFieldType,
    const uint32_t inByteOffset)
{
    myFieldName = inFieldName;
    myFieldType = inFieldType;
    myByteOffset = inByteOffset;
}

const std::string& ReflectedField::GetFieldName() const
{
    return myFieldName;
}

const std::string& ReflectedField::GetFieldType() const
{
    return myFieldType;
}

const uint32_t ReflectedField::GetByteOffset() const
{
    return myByteOffset;
}

nlohmann::json ReflectedField::Save() const
{
    nlohmann::json json;
    json["fieldName"] = myFieldName;
    json["fieldType"] = myFieldType;
    return json;
}

void ReflectedField::Load(const nlohmann::json& inJson)
{
    myFieldName = inJson["fieldName"];
    myFieldType = inJson["fieldType"];
}
