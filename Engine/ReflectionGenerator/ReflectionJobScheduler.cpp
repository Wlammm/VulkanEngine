#include "ReflectionJobScheduler.h"

#include <execution>
#include <filesystem>
#include <fstream>
#include <iostream>

ReflectionJobScheduler::ReflectionJobScheduler(const IncludePaths& inEngineIncludes, const IncludePaths& inEditorIncludes, const IncludePaths& inGameIncludes)
{
    const std::string engineProjectPath = "../Engine/";
    const std::string editorProjectPath = "../Editor/";
    const std::string gameProjectPath = "../Game/";

    GenerateUnityFile(inEngineIncludes, inEditorIncludes, inGameIncludes);
}

ReflectionJobScheduler::~ReflectionJobScheduler()
{
    delete myParser;
    myParser = nullptr;
}

void ReflectionJobScheduler::ExecuteParser()
{
    myParser->Parse();
}

const ReflectionParser* ReflectionJobScheduler::GetParser() const
{
    return myParser;
}

const std::vector<std::string>& ReflectionJobScheduler::GetHeaders() const
{
    return myHeaders;
}

void ReflectionJobScheduler::FindAllHeadersInDirectory(const std::string& inDirectory, std::vector<std::string>& outHeaders)
{
    if (!std::filesystem::exists(inDirectory) || !std::filesystem::is_directory(inDirectory))
    {
        std::cout << "ReflectionJobScheduler::FindAllHeadersInDirectory - inDirectory doesnt exists or isnt a directory." << std::endl;
        return;
    }
    
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(inDirectory))
    {
        if (std::filesystem::is_regular_file(entry) && (entry.path().extension() == ".h" || entry.path().extension() == ".hpp"))
            outHeaders.push_back(entry.path().string());
    }
}

void ReflectionJobScheduler::GenerateUnityFile(const IncludePaths& inEngineIncludes, const IncludePaths& inEditorIncludes, const IncludePaths& inGameIncludes)
{
    const std::string engineProjectPath = "../Engine/";
    const std::string editorProjectPath = "../Editor/";
    const std::string gameProjectPath = "../Game/";
    
    IncludePaths combinedPaths = inEngineIncludes + inEditorIncludes;
    combinedPaths = combinedPaths + inGameIncludes;

    FindAllHeadersInDirectory(engineProjectPath, myHeaders);
    FindAllHeadersInDirectory(editorProjectPath, myHeaders);
    FindAllHeadersInDirectory(gameProjectPath, myHeaders);

    std::ofstream file("ReflectionUnityFile.hpp");

    file << "#include \"../Engine/EnginePch.h\"\n";
    
    for (const std::string& path : myHeaders)
    {
        file << "#include \"" << path << "\"\n";
    }

    file.close();

    myParser = new ReflectionParser("ReflectionUnityFile.hpp", combinedPaths);
}
