#pragma once
#include <string>
#include <vector>

#include "ReflectedField.h"


class Class
{
public:
    Class(const std::string& inClassName);

    const std::string& GetClassName() const;

    const std::vector<ReflectedField>& GetFields() const;
    
    
    void AddField(const ReflectedField& inField);
    
private:
    std::string myClassName;

    std::vector<ReflectedField> myFields;
};
