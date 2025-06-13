#pragma once
#include <memory>

#include "IncludePaths.h"
#include "ReflectionParser.h"

class ReflectionJobScheduler
{
public:
    ReflectionJobScheduler(const IncludePaths& inEngineIncludes, const IncludePaths& inEditorIncludes, const IncludePaths& inGameIncludes);
    ~ReflectionJobScheduler();
    ReflectionJobScheduler(const ReflectionJobScheduler&) = delete;
    ReflectionJobScheduler& operator=(const ReflectionJobScheduler&) = delete;

    void ExecuteParser();

    const ReflectionParser* GetParser() const;

    const std::vector<std::string>& GetHeaders() const;
    
private:
    void FindAllHeadersInDirectory(const std::string& inDirectory, std::vector<std::string>& outHeaders);
    
    void GenerateUnityFile(const IncludePaths& inEngineIncludes, const IncludePaths& inEditorIncludes, const IncludePaths& inGameIncludes);

private:
    ReflectionParser* myParser = nullptr;
    std::vector<std::string> myHeaders{};
};
