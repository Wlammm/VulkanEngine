#pragma once
#include <iostream>
#include <chrono>

#include "IncludePaths.h"
#include "ReflectionFileGenerator.h"
#include "ReflectionJobScheduler.h"
#include "ReflectionParser.h"

void PrintResult(const ReflectionParser& inFile)
{
    if (inFile.Failed())
    {
        std::cout << "File failed: " << inFile.GetFile() << std::endl;

        for (const std::string& errorMessage : inFile.GetErrorMessages())
        {
            std::cout << errorMessage << std::endl;
        }
        return;
    }

    std::cout << "File succeeded: " << inFile.GetFile() << std::endl;
    for (const Class& foundClass : inFile.GetClasses())
    {
        std::cout << "Class: " << foundClass.GetClassName() << std::endl;

        for (const ReflectedField& field : foundClass.GetFields())
        {
            std::cout << " - Field: " << field.GetFieldType() << " " << field.GetFieldName() << std::endl;
        }
    }

    for (const std::string& errorMessage : inFile.GetErrorMessages())
    {
        std::cout << errorMessage << std::endl;
    }
}

// Project is working from the Build directory.
int main()
{
    auto startTime = std::chrono::high_resolution_clock::now();
    
    const IncludePaths engineIncludes = IncludePaths("engine_includes.txt");
    const IncludePaths editorIncludes = IncludePaths("editor_includes.txt");
    const IncludePaths gameIncludes = IncludePaths("game_includes.txt");

    ReflectionJobScheduler scheduler = ReflectionJobScheduler(engineIncludes, editorIncludes, gameIncludes);
    scheduler.ExecuteParsers();

    ReflectionFileGenerator(scheduler.GetParsers());
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Generated reflection data in " << duration.count() << " ms." << std::endl;

    return 0;
}
