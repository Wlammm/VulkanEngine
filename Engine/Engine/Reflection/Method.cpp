#include "EnginePch.h"
#include "Method.h"

const std::string& MethodArgument::GetArgumentName() const
{
    return myArgumentName;
}

const Class* MethodArgument::GetArgumentType() const
{
    return myArgumentType;
}

MethodArgument::MethodArgument(const std::string& inArgumentName, const Class* inArgumentType)
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
    return myMetadata.Contains(inMetadata);
}

Method::Method(const std::string& inMethodName, const Class* inReturnType, const Delegate<void*(void*, const List<void*>&)>& inInvoker, const List<MethodArgument>& inArguments)
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

