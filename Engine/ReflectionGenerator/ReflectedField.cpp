#include "ReflectedField.h"

ReflectedField::ReflectedField(const std::string& inFieldName, const std::string& inFieldType)
{
    myFieldName = inFieldName;
    myFieldType = inFieldType;
}

const std::string& ReflectedField::GetFieldName() const
{
    return myFieldName;
}

const std::string& ReflectedField::GetFieldType() const
{
    return myFieldType;
}
