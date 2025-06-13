#pragma once
#include <iostream>

#include "IncludePaths.h"
#include "ReflectFile.h"

void PrintResult(const ReflectFile& inFile)
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
    for (const ReflectedClass& foundClass : inFile.GetClasses())
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
    const IncludePaths engineIncludePaths = IncludePaths("engine_includes.txt");
    const IncludePaths editorIncludes = IncludePaths("editor_includes.txt");
    const IncludePaths gameIncludes = IncludePaths("game_includes.txt");
    
    ReflectFile reflectFile("../Engine/Components/TransformComponent.h", engineIncludePaths);
    reflectFile.Start();
    PrintResult(reflectFile);
    
}
