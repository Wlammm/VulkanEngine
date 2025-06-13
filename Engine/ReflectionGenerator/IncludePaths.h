#pragma once
#include <string>
#include <vector>

class IncludePaths
{
public:
    IncludePaths(const std::string& inBuildProjectIncludeFilePath);

    const std::vector<std::string>& GetIncludeArguments() const;

    friend IncludePaths operator+(const IncludePaths& inFirst, const IncludePaths& inSecond);    
private:
    void LoadFromFile(const std::string& inBuildProjectIncludeFilePath);

    std::string ExpandEnvironmentVariables(const std::string& inInput);
    
private:
    std::vector<std::string> myIncludeArguments;
};

IncludePaths operator+(const IncludePaths& inFirst, const IncludePaths& inSecond);
