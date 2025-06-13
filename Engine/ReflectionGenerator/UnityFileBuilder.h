#pragma once
#include "ReflectionParser.h"

class ReflectionCache;

class UnityFileBuilder
{
public:
    UnityFileBuilder(const ReflectionCache& inReflectionCache);
    const std::vector<std::string>& GetHeaders() const;
    const std::string& GetPath() const;

    bool HasFilesToCompile() const;
    
private:
    void FindAllHeadersToReflectInDirectory(const std::string& inDirectory, const ReflectionCache& inReflectionCache, std::vector<std::string>& outHeaders);
    
    void GenerateUnityFile(const ReflectionCache& inReflectionCache);

private:
    std::vector<std::string> myHeaders{};
};
