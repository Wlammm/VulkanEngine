#include "ReflectionJobScheduler.h"

#include <execution>
#include <filesystem>
#include <iostream>

ReflectionJobScheduler::ReflectionJobScheduler(const IncludePaths& inEngineIncludes, const IncludePaths& inEditorIncludes, const IncludePaths& inGameIncludes)
{
    const std::string engineProjectPath = "../Engine/";
    const std::string editorProjectPath = "../Editor/";
    const std::string gameProjectPath = "../Game/";

    
    
    GenerateParsersForProject(engineProjectPath, inEngineIncludes);
    GenerateParsersForProject(editorProjectPath, inEditorIncludes);
    GenerateParsersForProject(gameProjectPath, inGameIncludes);
}

void ReflectionJobScheduler::ExecuteParsers()
{
    std::for_each(std::execution::par, myParsers.begin(), myParsers.end(), [](auto&& inParser)
    {
        inParser.ParseInParallel();
    });
}

const std::vector<ReflectionParser>& ReflectionJobScheduler::GetParsers() const
{
    return myParsers;
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

void ReflectionJobScheduler::GenerateParsersForProject(const std::string& inProjectRoot, const IncludePaths& inIncludePaths)
{
    std::vector<std::string> filesToParse{};
    FindAllHeadersInDirectory(inProjectRoot, filesToParse);
    for (const std::string& header : filesToParse)
    {
        myParsers.emplace_back(header, inIncludePaths);
    }
}
