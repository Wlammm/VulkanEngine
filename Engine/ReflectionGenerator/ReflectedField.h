#pragma once
#include <string>

#include "json.hpp"

class ReflectedField
{
public:
    ReflectedField();
    ReflectedField(const std::string& inFieldName, const std::string& inFieldType);

    const std::string& GetFieldName() const;
    const std::string& GetFieldType() const;

    nlohmann::json Save() const;
    void Load(const nlohmann::json& inJson);
    
private:
    std::string myFieldName;
    std::string myFieldType;
};
