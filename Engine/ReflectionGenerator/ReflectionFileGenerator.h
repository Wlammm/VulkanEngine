#pragma once
#include <vector>

#include "ReflectionParser.h"

class ReflectionFileGenerator
{
public:
    ReflectionFileGenerator(const std::vector<ReflectionParser>& inReflectionParsers);

private:
    void BuildClassIncludes(const std::vector<ReflectionParser>& inReflectionParsers, std::string& outString);
    void BuildClassDeclarations(const std::vector<ReflectionParser>& inReflectionParsers, std::string& outString);
    void BuildClassContentDeclarations(const std::vector<ReflectionParser>& inReflectionParsers, std::string& outString);
    
    void CreateFileContent(const std::string& inIncludes, const std::string& inClassDeclarations, const std::string& inClassContentDeclarations, std::string& outContent);
};
