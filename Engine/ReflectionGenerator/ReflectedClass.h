#pragma once
#include <string>
#include <vector>

#include "json.hpp"
#include "ReflectedField.h"
#include "ReflectedMethod.h"

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
    
    void AddField(const ReflectedField& inField);
    void AddMethod(const ReflectedMethod& inMethod);
    
private:
    // The file this class is defined in.
    std::string myClassName;
    
    std::vector<ReflectedField> myFields;
    std::vector<ReflectedMethod> myMethods;
    
    std::vector<std::string> myBaseTypeNames;
};
