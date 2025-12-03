#pragma once
#include <iostream>
#include <chrono>
#include <thread>

#include "IncludePaths.h"
#include "ReflectionCache.h"
#include "ReflectionFileBuilder.h"
#include "UnityFileBuilder.h"
#include "ReflectionParser.h"

void PrintResult(const ReflectionParser& inParser)
{
    if (inParser.Failed())
    {
        std::cout << "File failed: " << inParser.GetFile() << std::endl;

        for (const std::string& errorMessage : inParser.GetErrorMessages())
        {
            std::cout << errorMessage << std::endl;
        }
        return;
    }

    std::cout << "File succeeded: " << inParser.GetFile() << std::endl;
    for (const std::list<ReflectedClass>& classList : inParser.GetClassData() | std::views::values)
    {
        for (const ReflectedClass& foundClass : classList)
        {
            std::cout << "Class: " << foundClass.GetClassName() << std::endl;

            for (const ReflectedField& field : foundClass.GetFields())
            {
                std::cout << " - Field: " << field.GetFieldType() << " " << field.GetFieldName() << std::endl;
            }
        }
    }

    for (const std::string& errorMessage : inParser.GetErrorMessages())
    {
        std::cout << errorMessage << std::endl;
    }
}

// Project is working from the Build directory.
int main(int argc, char* argv[])
{
    bool isDebugBuild = false;

    if (argc > 1)
    {
        if (std::string(argv[1]) == "--debug")
        {
            isDebugBuild = true;
        }
    }

    if (isDebugBuild)
        std::cout << "Generating reflection data for debug configuration." << std::endl;
    else
        std::cout << "Generating reflection data for release configuration" << std::endl;
    
    auto startTime = std::chrono::high_resolution_clock::now();

    std::string cacheFilePath = "ReflectionCache.json";
    if (isDebugBuild)
        cacheFilePath = "Debug_ReflectionCache.json";
    ReflectionCache reflectionCache(cacheFilePath);
    
    IncludePaths includes = IncludePaths("engine_includes.txt") + IncludePaths("editor_includes.txt") + IncludePaths("game_includes.txt");

    UnityFileBuilder unityFile = UnityFileBuilder(reflectionCache);

    if (unityFile.HasFilesToCompile())
    {
        ReflectionParser parser = ReflectionParser(unityFile.GetPath(), includes, isDebugBuild);
    
        reflectionCache.UpdateCacheFromParser(parser);
        reflectionCache.UpdateCacheFromUnityFile(unityFile);
        
        // Only save to cache for actual runs. We dont want it when debugging.
        if (argc > 1)
            reflectionCache.SaveCache();
    
        ReflectionFileBuilder fileBuilder(reflectionCache);
    }
    
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Generated reflection data in " << duration.count() << " ms." << std::endl;

    return 0;
}
