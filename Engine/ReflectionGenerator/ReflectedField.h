#pragma once
#include <string>

#include "json.hpp"

class ReflectedField
{
public:
    ReflectedField();
    ReflectedField(const std::string& inFieldName, const std::string& inFieldType, const uint32_t inByteOffset, const std::vector<std::string>& inMetadata);

    const std::string& GetFieldName() const;
    const std::string& GetFieldType() const;

    const std::vector<std::string>& GetFieldMetadata() const;

    const uint32_t GetByteOffset() const;

    nlohmann::json Save() const;
    void Load(const nlohmann::json& inJson);
    
private:
    std::string myFieldName;
    std::string myFieldType;

    std::vector<std::string> myMetadata;

    uint32_t myByteOffset;
};
