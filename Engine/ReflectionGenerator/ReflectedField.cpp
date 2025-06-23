#include "ReflectedField.h"

ReflectedField::ReflectedField()
{
}

ReflectedField::ReflectedField(const std::string& inFieldName, const std::string& inFieldType, const uint32_t inByteOffset, const std::vector<std::string>& inMetadata)
{
    myFieldName = inFieldName;
    myFieldType = inFieldType;
    myByteOffset = inByteOffset;
    myMetadata = inMetadata;
}

const std::string& ReflectedField::GetFieldName() const
{
    return myFieldName;
}

const std::string& ReflectedField::GetFieldType() const
{
    return myFieldType;
}

const std::vector<std::string>& ReflectedField::GetFieldMetadata() const
{
    return myMetadata;
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
    json["byteOffset"]  = myByteOffset;

    for (const std::string& metadata : myMetadata)
    {
        json["metadata"].push_back(metadata);
    }

    for (const std::string& metadata : myTemplateArguments)
    {
        json["templateArguments"].push_back(metadata);
    }
    
    return json;
}

void ReflectedField::Load(const nlohmann::json& inJson)
{
    myFieldName = inJson["fieldName"];
    myFieldType = inJson["fieldType"];
    myByteOffset = inJson["byteOffset"];
    if(inJson.contains("metadata"))
        myMetadata = inJson["metadata"].get<std::vector<std::string>>();

    if (inJson.contains("templateArguments"))
        myTemplateArguments = inJson["templateArguments"].get<std::vector<std::string>>();
}

void ReflectedField::AddTemplateArgument(const std::string& inTemplateArgument)
{
    myTemplateArguments.emplace_back(inTemplateArgument);
}

const std::vector<std::string>& ReflectedField::GetTemplateArguments() const
{
    return myTemplateArguments;
}
