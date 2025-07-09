#include "UnityFileBuilder.h"

#include <execution>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "ReflectionCache.h"

UnityFileBuilder::UnityFileBuilder(const ReflectionCache& inReflectionCache)
{
    GenerateUnityFile(inReflectionCache);
}

const std::vector<std::string>& UnityFileBuilder::GetHeaders() const
{
    return myHeaders;
}

const std::string& UnityFileBuilder::GetPath() const
{
    static std::string path = "ReflectionUnityFile.hpp";
    return path;
}

bool UnityFileBuilder::HasFilesToCompile() const
{
    return myHasFilesToCompile;
}

void UnityFileBuilder::FindAllHeadersToReflectInDirectory(const std::string& inDirectory, const ReflectionCache& inReflectionCache, std::vector<std::string>& outHeaders)
{
    if (!std::filesystem::exists(inDirectory) || !std::filesystem::is_directory(inDirectory))
    {
        std::cout << "ReflectionJobScheduler::FindAllHeadersInDirectory - inDirectory doesnt exists or isnt a directory." << std::endl;
        return;
    }

    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(inDirectory))
    {
        if (std::filesystem::is_regular_file(entry) && (entry.path().extension() == ".h" || entry.path().extension() == ".hpp"))
        {
            if (!inReflectionCache.IsCacheValidForFile(entry.path().string()))
            {
                // TODO: The reflection system sometimes breaks for template instantiations as their declaration might not be part of the compile. For now we regenerate for the whole project as skipping some classes are neglible performance wise..
                myHasFilesToCompile = true;
            }
            
            outHeaders.push_back(entry.path().string());
        }
    }
}

void UnityFileBuilder::GenerateUnityFile(const ReflectionCache& inReflectionCache)
{
    const std::string engineProjectPath = "../Engine/";
    const std::string editorProjectPath = "../Editor/";
    const std::string gameProjectPath = "../Game/";
    
    FindAllHeadersToReflectInDirectory(engineProjectPath, inReflectionCache, myHeaders);
    FindAllHeadersToReflectInDirectory(editorProjectPath, inReflectionCache, myHeaders);
    FindAllHeadersToReflectInDirectory(gameProjectPath, inReflectionCache, myHeaders);
    
    std::ofstream file("ReflectionUnityFile.hpp");
    file << "#include \"../Engine/EnginePch.h\"\n";
    for (const std::string& path : myHeaders)
    {
        file << "#include \"" << path << "\"\n";
    }
    file.close();
}
