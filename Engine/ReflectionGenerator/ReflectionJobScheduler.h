#pragma once
#include <memory>

#include "IncludePaths.h"
#include "ReflectionParser.h"

class ReflectionJobScheduler
{
public:
    ReflectionJobScheduler(const IncludePaths& inEngineIncludes, const IncludePaths& inEditorIncludes, const IncludePaths& inGameIncludes);

    void ExecuteParsers();

    const std::vector<ReflectionParser>& GetParsers() const;
    
private:
    void FindAllHeadersInDirectory(const std::string& inDirectory, std::vector<std::string>& outHeaders);
    
    void GenerateParsersForProject(const std::string& inProjectRoot, const IncludePaths& inIncludePaths);
    
private:
    std::vector<ReflectionParser> myParsers{};
};
