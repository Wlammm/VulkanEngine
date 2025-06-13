#pragma once
#include <string>

class ReflectionCache;

class ReflectionFileBuilder
{
public:
    ReflectionFileBuilder(const ReflectionCache& inCache);

private:
    void BuildClassIncludes(const ReflectionCache& inCache, std::string& outString);
    void BuildClassDeclarations(const ReflectionCache& inCache, std::string& outString);
    void BuildClassContentDeclarations(const ReflectionCache& inCache, std::string& outString);
    
    void CreateFileContent(const std::string& inIncludes, const std::string& inClassDeclarations, const std::string& inClassContentDeclarations, std::string& outContent);
};
