#pragma once
#include <algorithm>
#include <string>

class PathUtils
{
public:
    static std::string NormalizePath(const std::string& input)
    {
        std::string result = input;

        // Remove .\ prefix if it exists
        const std::string prefix = ".\\";
        if (result.rfind(prefix, 0) == 0)
        {
            result = result.substr(prefix.length());
        }

        // Replace all backslashes with forward slashes
        std::replace(result.begin(), result.end(), '\\', '/');

        return result;
    }
};
