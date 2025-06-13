#include "IncludePaths.h"
#include <fstream>
#include <sstream>
#include <filesystem>

IncludePaths::IncludePaths(const std::string& inBuildProjectIncludeFilePath)
{
    LoadFromFile(inBuildProjectIncludeFilePath);
}

const std::vector<std::string>& IncludePaths::GetIncludeArguments() const
{
    return myIncludePaths;
}

void IncludePaths::LoadFromFile(const std::string& inBuildProjectIncludeFilePath)
{
    std::ifstream fileStream(inBuildProjectIncludeFilePath);

    std::string line;
    while (std::getline(fileStream, line))
    {
        std::string includeArg = "-I" + line;
        myIncludePaths.emplace_back(includeArg);
    }

    fileStream.close();
}