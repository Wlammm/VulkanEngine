#include "ReflectedClass.h"

ReflectedClass::ReflectedClass(const std::string& inClassName)
{
    myClassName = inClassName;
}

const std::string& ReflectedClass::GetClassName() const
{
    return myClassName;
}

const std::vector<ReflectedField>& ReflectedClass::GetFields() const
{
    return myFields;
}

void ReflectedClass::AddField(const ReflectedField& inField)
{
    myFields.emplace_back(inField);
}
