#pragma once
#include <vector>

#include "ReflectionParser.h"

class ReflectionFileGenerator
{
public:
    ReflectionFileGenerator(const std::vector<std::unique_ptr<ReflectionParser>>& inReflectionParsers);

private:
    void BuildClassIncludes(const std::vector<std::unique_ptr<ReflectionParser>>& inReflectionParsers, std::string& outString);
    void BuildClassDeclarations(const std::vector<std::unique_ptr<ReflectionParser>>& inReflectionParsers, std::string& outString);
    void BuildClassContentDeclarations(const std::vector<std::unique_ptr<ReflectionParser>>& inReflectionParsers, std::string& outString);
    
    void CreateFileContent(const std::string& inIncludes, const std::string& inClassDeclarations, const std::string& inClassContentDeclarations, std::string& outContent);
};
