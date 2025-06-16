#include "ReflectedMethod.h"

ReflectedMethodArgument::ReflectedMethodArgument()
{
}

ReflectedMethodArgument::ReflectedMethodArgument(const std::string& inArgumentName, const std::string& inArgumentType, const std::string& inUnqualifiedTypeName, const bool inIsPointer, const bool inIsReference)
{
    myArgumentName = inArgumentName;
    myArgumentType = inArgumentType;
    myIsPointer = inIsPointer;
    myIsReference = inIsReference;
    myUnqualifiedArgumentTypeName = inUnqualifiedTypeName;
}

nlohmann::json ReflectedMethodArgument::Save() const
{
    nlohmann::json json;
    json["ArgumentName"] = myArgumentName;
    json["ArgumentType"] = myArgumentType;
    json["IsPointer"] = myIsPointer;
    json["IsReference"] = myIsReference;
    json["UnqualifiedArgumentTypeName"] = myUnqualifiedArgumentTypeName;
    return json;
}

void ReflectedMethodArgument::Load(const nlohmann::json& inJson)
{
    myArgumentName = inJson["ArgumentName"];
    myArgumentType = inJson["ArgumentType"];
    myIsPointer = inJson["IsPointer"];
    myIsReference = inJson["IsReference"];
    myUnqualifiedArgumentTypeName = inJson["UnqualifiedArgumentTypeName"];
}

const std::string& ReflectedMethodArgument::GetArgumentName() const
{
    return myArgumentName;
}

const std::string& ReflectedMethodArgument::GetArgumentType() const
{
    return myArgumentType;
}

const std::string& ReflectedMethodArgument::GetUnqualifiedTypeName() const
{
    return myUnqualifiedArgumentTypeName;
}

const bool ReflectedMethodArgument::GetIsPointer() const
{
    return myIsPointer;
}

const bool ReflectedMethodArgument::GetIsReference() const
{
    return myIsReference;
}

ReflectedMethod::ReflectedMethod()
{
}

ReflectedMethod::ReflectedMethod(const std::string& inMethodName, const std::string& inReturnTypeName, const bool inIsConst, const bool inIsStatic, const bool inIsReturnTypePointer, const bool inIsReturnTypeReference, const std::vector<std::string>& inMetadata)
{
    myMethodName = inMethodName;
    myReturnTypeName = inReturnTypeName;
    myIsConst = inIsConst;
    myIsStatic = inIsStatic;
    myIsReturnTypePointer = inIsReturnTypePointer;
    myIsReturnTypeReference = inIsReturnTypeReference;
    myMetadata = inMetadata;
}

void ReflectedMethod::AddArgument(const ReflectedMethodArgument& inArgument)
{
    myArguments.push_back(inArgument);
}

const std::string& ReflectedMethod::GetMethodName() const
{
    return myMethodName;
}

const std::string& ReflectedMethod::GetReturnTypeName() const
{
    return myReturnTypeName;
}

const std::vector<ReflectedMethodArgument>& ReflectedMethod::GetArguments() const
{
    return myArguments;
}

const bool ReflectedMethod::IsReturnTypePointer() const
{
    return myIsReturnTypePointer;
}

const bool ReflectedMethod::IsReturnTypeReference() const
{
    return myIsReturnTypeReference;
}

const std::vector<std::string>& ReflectedMethod::GetMetadata() const
{
    return myMetadata;
}

nlohmann::json ReflectedMethod::Save() const
{
    nlohmann::json json;
    json["methodName"] = myMethodName;
    json["returnTypeName"] = myReturnTypeName;
    json["isConst"] = myIsConst;
    json["isStatic"] = myIsStatic;
    json["isReturnTypePointer"] = myIsReturnTypePointer;
    json["isReturnTypeReference"] = myIsReturnTypeReference;
    json["metadata"] = myMetadata;

    for (const ReflectedMethodArgument& argument : myArguments)
    {
        json["arguments"].push_back(argument.Save());
    }
    
    return json;
}

void ReflectedMethod::Load(const nlohmann::json& inJson)
{
    myMethodName = inJson["methodName"].get<std::string>();
    myReturnTypeName = inJson["returnTypeName"].get<std::string>();
    myIsConst = inJson["isConst"].get<bool>();
    myIsStatic = inJson["isStatic"].get<bool>();
    myIsReturnTypePointer = inJson["isReturnTypePointer"].get<bool>();
    myIsReturnTypeReference = inJson["isReturnTypeReference"].get<bool>();
    myMetadata = inJson["metadata"].get<std::vector<std::string>>();

    if (inJson.contains("arguments"))
    {
        for (const nlohmann::json& json : inJson["arguments"])
        {
            ReflectedMethodArgument argument;
            argument.Load(json);
            myArguments.push_back(argument);
        }
    }
}

bool ReflectedMethod::IsConst() const
{
    return myIsConst;
}

bool ReflectedMethod::IsStatic() const
{
    return myIsStatic;
}
