#pragma once

#include <string>

class ReflectedClass;

class Field
{
public:
    const std::string& GetName() const;
    uint32_t GetOffset() const;
    const ReflectedClass* GetType() const;

private:
    friend class GeneratedReflectionData;
    Field() = delete;
    Field(const std::string& inName, const uint32_t inOffset, const ReflectedClass* inType, const bool inIsPointer, const bool inIsReference);
    
    std::string myName = "";
    uint32_t myOffset = 0;
    const ReflectedClass* myType = nullptr;
    bool myIsPointer = false;
    bool myIsReference = false;
};
