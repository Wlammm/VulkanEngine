#include "IncludePaths.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>

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
        line = ExpandEnvironmentVariables(line);
        std::string includeArg = "-I" + line;
        myIncludePaths.emplace_back(includeArg);
    }

    fileStream.close();
}

std::optional<std::string> GetEnvVariable(const std::string& varName)
{
#ifdef _WIN32
    char* buffer = nullptr;
    size_t size = 0;
    if (_dupenv_s(&buffer, &size, varName.c_str()) == 0 && buffer != nullptr)
    {
        std::string value(buffer);
        free(buffer);
        return value;
    }
    return std::nullopt;
#else
    const char* val = std::getenv(varName.c_str());
    if (val == nullptr)
        return std::nullopt;
    return std::string(val);
#endif
}

std::string IncludePaths::ExpandEnvironmentVariables(const std::string& inInput)
{
    static const std::regex envPattern(R"(%\{([A-Za-z_][A-Za-z0-9_]*)\})");
    std::string result;
    std::string::const_iterator searchStart(inInput.cbegin());
    std::smatch match;

    while (std::regex_search(searchStart, inInput.cend(), match, envPattern))
    {
        result.append(searchStart, match[0].first);
        std::string varName = match[1].str();

        std::string envValue = GetEnvVariable(varName).value_or(varName);
        result.append(envValue);
        searchStart = match[0].second;
    }
    result.append(searchStart, inInput.cend());
    return result;
}
