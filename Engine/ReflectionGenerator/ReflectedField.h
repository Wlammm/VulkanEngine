#pragma once
#include <string>

#include "json.hpp"

class ReflectedField
{
public:
    ReflectedField();
    ReflectedField(const std::string& inFieldName, const std::string& inFieldType, const uint32_t inByteOffset);

    const std::string& GetFieldName() const;
    const std::string& GetFieldType() const;

    const uint32_t GetByteOffset() const;

    nlohmann::json Save() const;
    void Load(const nlohmann::json& inJson);
    
private:
    std::string myFieldName;
    std::string myFieldType;

    uint32_t myByteOffset;
};
