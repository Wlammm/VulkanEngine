#include "EnginePch.h"
#include "Field.h"

#include "Class.h"

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

const List<std::string>& Field::GetMetadata() const
{
    return myMetadata;
}

bool Field::HasMetadata(const std::string& inMetadata) const
{
    for (const std::string& metadata : myMetadata)
    {
        if (metadata.contains(inMetadata))
            return true;
    }

    return false;
}

List<std::string> Field::GetMetadataArgs(const std::string& inMetadata) const
{
    for (const std::string& metadata : myMetadata)
    {
        if (metadata.contains(inMetadata))
        {
            List<std::string> args;
            size_t openParen = metadata.find('(');
            size_t closeParen = metadata.find(')', openParen);
            if (openParen == std::string::npos || closeParen == std::string::npos || closeParen <= openParen)
                return args;

            std::string argsStr = metadata.substr(openParen + 1, closeParen - openParen - 1);
            std::stringstream ss(argsStr);
            std::string arg;

            while (std::getline(ss, arg, ','))
            {
                arg.erase(0, arg.find_first_not_of(" \t\n\r"));
                arg.erase(arg.find_last_not_of(" \t\n\r") + 1);
                args.Add(arg);
            }

            return args;
        }
    }
    return {};
}


Field::Field(const std::string& inName, const uint inOffset, const Class* inType, const bool inIsPointer, const bool inIsReference)
{
    myName = inName;
    myOffset = inOffset;
    myType = inType;
    myIsPointer = inIsPointer;
    myIsReference = inIsReference;
}

void Field::AddMetadata(const std::string& inMetadata)
{
    myMetadata.Add(inMetadata);
}
