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

    const std::vector<std::string>& GetMetadata() const;
    bool HasMetadata(const std::string& inMetadata) const;
    
private:
    friend class GeneratedReflectionData;
    Field() = delete;
    Field(const std::string& inName, const uint32_t inOffset, const Class* inType);

    void AddMetadata(const std::string& inMetadata);
    
    std::string myName = "";
    uint32_t myOffset = 0;
    const Class* myType = nullptr;

    // TODO: Replacing this vector with a list shows a bug in the list class when moving elements. I dont wanna fix it right now but should probably be done soon.
    std::vector<std::string> myMetadata;
};
