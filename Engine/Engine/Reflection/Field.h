#pragma once

class Class;

class Field
{
public:
    const std::string& GetName() const;
    uint GetOffset() const;
    const Class* GetType() const;

private:
    friend class GeneratedReflectionData;
    Field() = delete;
    Field(const std::string& inName, const uint inOffset, const Class* inType, const bool inIsPointer, const bool inIsReference);
    
    std::string myName = "";
    uint myOffset = 0;
    const Class* myType = nullptr;
    bool myIsPointer = false;
    bool myIsReference = false;
};
