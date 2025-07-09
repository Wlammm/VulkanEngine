#include "EnginePch.h"
#include "Method.h"

const std::string& MethodArgument::GetArgumentName() const
{
    return myArgumentName;
}

const Type* MethodArgument::GetArgumentType() const
{
    return myArgumentType;
}

MethodArgument::MethodArgument(const std::string& inArgumentName, const Type* inArgumentType)
{
    myArgumentName = inArgumentName;
    myArgumentType = inArgumentType;
}

const std::string& Method::GetMethodName() const
{
    return myMethodName;
}

const List<std::string>& Method::GetMetadata() const
{
    return myMetadata;
}

bool Method::HasMetadata(const std::string& inMetadata) const
{
    for (const std::string& metadata : myMetadata)
    {
        if (metadata.contains(inMetadata))
            return true;
    }
    return false;
}

List<std::string> Method::GetMetadataArgs(const std::string& inMetadata) const
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

Method::Method(const std::string& inMethodName, const Type* inReturnType, const Delegate<void*(void*, const List<void*>&)>& inInvoker, const List<MethodArgument>& inArguments)
{
    myMethodName = inMethodName;
    myReturnType = inReturnType;
    myArguments = inArguments;
    myInvoker = std::move(inInvoker);
}

void Method::AddMetadata(const std::string& inMetadata)
{
    myMetadata.Add(inMetadata);
}

