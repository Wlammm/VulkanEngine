#pragma once
#include <string>

#include "json.hpp"

class ReflectedMethodArgument
{
public:
    ReflectedMethodArgument();
    ReflectedMethodArgument(const std::string& inArgumentName, const std::string& inArgumentType, const std::string& inUnqualifiedTypeName, const bool inIsPointer, const bool inIsReference);

    nlohmann::json Save() const;
    void Load(const nlohmann::json& inJson);

    const std::string& GetArgumentName() const;
    const std::string& GetArgumentType() const;
    const std::string& GetUnqualifiedTypeName() const;
    const bool GetIsPointer() const;
    const bool GetIsReference() const;
    
private:
    std::string myArgumentName;
    std::string myArgumentType;
    std::string myUnqualifiedArgumentTypeName;

    bool myIsPointer = false;
    bool myIsReference = false;
};

class ReflectedMethod
{
public:
    ReflectedMethod();
    ReflectedMethod(const std::string& inMethodName, const std::string& inReturnTypeName, const bool inIsConst, const bool inIsStatic, const bool inIsReturnTypePointer, const bool inIsReturnTypeReference, const std::vector<std::string>& inMetadata);

    void AddArgument(const ReflectedMethodArgument& inArgument);

    const std::string& GetMethodName() const;
    const std::string& GetReturnTypeName() const;
    const std::vector<ReflectedMethodArgument>& GetArguments() const;

    const bool IsReturnTypePointer() const;
    const bool IsReturnTypeReference() const;

    const std::vector<std::string>& GetMetadata() const;

    
    nlohmann::json Save() const;

    void Load(const nlohmann::json& inJson);

    bool IsConst() const;
    bool IsStatic() const;
    
private:
    std::string myMethodName;
    std::string myReturnTypeName;

    bool myIsReturnTypePointer = false;
    bool myIsReturnTypeReference = false;
    
    bool myIsConst = false;
    bool myIsStatic = false;
    
    std::vector<ReflectedMethodArgument> myArguments;
    std::vector<std::string> myMetadata;
};
