#include "EnginePch.h"
#include "Field.h"

const std::string& Field::GetName() const
{
    return myName;
}

uint Field::GetOffset() const
{
    return myOffset;
}

const Class* Field::GetType() const
{
    return myType;
}

Field::Field(const std::string& inName, const uint inOffset, const Class* inType, const bool inIsPointer, const bool inIsReference)
{
    myName = inName;
    myOffset = inOffset;
    myType = inType;
    myIsPointer = inIsPointer;
    myIsReference = inIsReference;
}
