#include "Class.h"

Class::Class(const std::string& inClassName)
{
    myClassName = inClassName;
}

const std::string& Class::GetClassName() const
{
    return myClassName;
}

const std::vector<ReflectedField>& Class::GetFields() const
{
    return myFields;
}

void Class::AddField(const ReflectedField& inField)
{
    myFields.emplace_back(inField);
}
