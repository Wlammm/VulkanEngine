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

const std::vector<std::string>& Field::GetMetadata() const
{
    return myMetadata;
}

bool Field::HasMetadata(const std::string& inMetadata) const
{
    return std::find(myMetadata.begin(), myMetadata.end(), inMetadata) != myMetadata.end();
}

Field::Field(const std::string& inName, const uint inOffset, const Class* inType)
{
    myName = inName;
    myOffset = inOffset;
    myType = inType;
}

void Field::AddMetadata(const std::string& inMetadata)
{
    myMetadata.push_back(inMetadata);
}
