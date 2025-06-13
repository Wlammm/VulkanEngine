#pragma once
#include <string>

class ReflectedField
{
public:
    ReflectedField(const std::string& inFieldName, const std::string& inFieldType);

    const std::string& GetFieldName() const;
    const std::string& GetFieldType() const;

private:
    std::string myFieldName;
    std::string myFieldType;
};
