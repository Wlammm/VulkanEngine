#pragma once

#include "Engine/Containers/List.hpp"
#include <string>

class Class;

class Field
{
public:
    const std::string& GetName() const;
    uint32_t GetOffset() const;
    const Class* GetType() const;

    const List<std::string>& GetMetadata() const;
    bool HasMetadata(const std::string& inMetadata) const;
    
private:
    friend class GeneratedReflectionData;
    Field() = delete;
    Field(const std::string& inName, const uint32_t inOffset, const Class* inType);

    void AddMetadata(const std::string& inMetadata);
    
    std::string myName = "";
    uint32_t myOffset = 0;
    const Class* myType = nullptr;

    List<std::string> myMetadata;
};
