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

    // Returns the arguments of a metadata tag. Example META(SomeMeta(Arg1, Arg2)) returns Arg1 and Arg2
    List<std::string> GetMetadataArgs(const std::string& inMetadata) const;

    template<typename Type>
    Type& GetTypeFromInstance(void* inInstance) const
    {
        char* charPtr = (char*)inInstance;
        Type* typePtr = (Type*)(charPtr + myOffset);
        return *typePtr;
    }

    void* GetPointerToValue(void* inInstance) const
    {
        char* charPtr = (char*)inInstance;
        return (void*)(charPtr + myOffset);
    }

private:
    friend class GeneratedReflectionData;
    Field() = delete;
    Field(const std::string& inName, const uint32_t inOffset, const Class* inType, const bool inIsPointer, const bool inIsReference);

    void AddMetadata(const std::string& inMetadata);
    
    std::string myName = "";
    uint32_t myOffset = 0;
    const Class* myType = nullptr;

    bool myIsPointer = false;
    bool myIsReference = false;
    
    List<std::string> myMetadata;
};
