#pragma once
#include <string>
#include <vector>

class IncludePaths
{
public:
    IncludePaths(const std::string& inBuildProjectIncludeFilePath);

    const std::vector<std::string>& GetIncludeArguments() const;

private:
    void LoadFromFile(const std::string& inBuildProjectIncludeFilePath);
    
private:
    std::vector<std::string> myIncludePaths;
};
