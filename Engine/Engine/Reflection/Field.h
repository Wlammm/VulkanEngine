#pragma once

#include <string>

class Class;

class Field
{
public:
    const std::string& GetName() const;
    uint32_t GetOffset() const;
    const Class* GetType() const;

private:
    friend class GeneratedReflectionData;
    Field() = delete;
    Field(const std::string& inName, const uint32_t inOffset, const Class* inType, const bool inIsPointer, const bool inIsReference);
    
    std::string myName = "";
    uint32_t myOffset = 0;
    const Class* myType = nullptr;
    bool myIsPointer = false;
    bool myIsReference = false;
};
