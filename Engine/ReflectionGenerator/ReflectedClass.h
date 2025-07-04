#pragma once
#include <string>
#include <vector>

#include "json.hpp"
#include "ReflectedField.h"
#include "ReflectedMethod.h"

struct ReflectedClassTemplateArgument
{
    std::string myTypeName;
    bool myIsPointer;
    bool myIsReference;
};

class ReflectedClass
{
public:
    ReflectedClass();
    ReflectedClass(const std::string& inClassName);

    const std::string& GetClassName() const;

    const std::vector<ReflectedField>& GetFields() const;
    const std::vector<std::string>& GetBaseTypeNames() const;
    
    const std::vector<ReflectedMethod>& GetMethods() const;
    
    void AddBaseClass(const std::string& inBaseTypeName);    

    nlohmann::json Save() const;
    void Load(const nlohmann::json& inJson);
    
    ReflectedField& AddField(const ReflectedField& inField);
    void AddMethod(const ReflectedMethod& inMethod);

    void AddTemplateArgument(const int inIndex, const std::string& inTemplateArgument, const bool inIsPointer, const bool inIsReference);
    const std::vector<ReflectedClassTemplateArgument>& GetTemplateArguments() const;
    

private:
    // The file this class is defined in.
    std::string myClassName;
    
    std::vector<ReflectedField> myFields;
    std::vector<ReflectedMethod> myMethods;
    
    std::vector<std::string> myBaseTypeNames;
    std::vector<ReflectedClassTemplateArgument> myTemplateArguments;
};
