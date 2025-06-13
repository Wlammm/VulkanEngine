#pragma once
#include <vector>

#include "ReflectionParser.h"

class ReflectionJobScheduler;

class ReflectionFileBuilder
{
public:
    ReflectionFileBuilder(const ReflectionParser* inReflectionParser, const ReflectionJobScheduler& inScheduler);

private:
    void BuildClassIncludes(const ReflectionJobScheduler& inScheduler, std::string& outString);
    void BuildClassDeclarations(const ReflectionParser* inReflectionParsers, std::string& outString);
    void BuildClassContentDeclarations(const ReflectionParser* inReflectionParsers, std::string& outString);
    
    void CreateFileContent(const std::string& inIncludes, const std::string& inClassDeclarations, const std::string& inClassContentDeclarations, std::string& outContent);
};
