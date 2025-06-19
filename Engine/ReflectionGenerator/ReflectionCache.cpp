#include "ReflectionCache.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "json.hpp"
#include "PathUtils.hpp"
#include "ReflectionParser.h"

ReflectionCache::ReflectionCache(const std::filesystem::path& inCachePath)
{
    myCachePath = inCachePath;
    
    LoadCache();
}

void ReflectionCache::SaveCache()
{
    nlohmann::json json;

    for (auto& [filePath, cachedData] : myCache)
    {
        json[filePath] = cachedData.Save();
    }

    std::ofstream file(myCachePath);
    file << json;
    file.close();
}

void ReflectionCache::LoadCache()
{
    if (!std::filesystem::exists(myCachePath))
        return;
    
    std::ifstream file(myCachePath);
    nlohmann::json json = nlohmann::json::parse(file);
    file.close();
    
    for (auto& [filePath, cacheData] : json.items())
    {
        if (!std::filesystem::exists(filePath))
            continue;
        
        CachedFileData fileData;
        fileData.Load(cacheData);
        myCache[filePath] = fileData;
    }
}

void ReflectionCache::CacheFile(const std::string& inFile, const std::list<ReflectedClass>& inClasses)
{
    std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(inFile); 

    myCache[inFile] = { lastWriteTime, inClasses };
}

void ReflectionCache::UpdateWriteTimeForFile(const std::string& inFile)
{
    std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(inFile);
    myCache[inFile].myLastWriteTime = lastWriteTime;
}

bool ReflectionCache::IsCacheValidForFile(const std::string& inFile) const
{
    const std::string normalizedFilePath = PathUtils::NormalizePath(inFile);
    if (myCache.find(normalizedFilePath) == myCache.end())
        return false;
    
    std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(normalizedFilePath);
    return myCache.at(normalizedFilePath).myLastWriteTime == lastWriteTime;
}

const std::list<ReflectedClass>& ReflectionCache::GetCachedClassesForFile(const std::string& inFile) const
{
    return myCache.at(inFile).myClasses;
}

void ReflectionCache::UpdateCacheFromParser(const ReflectionParser& inParser)
{
    for (const auto& [filePath, classesInFile] : inParser.GetClassData())
    {
        CacheFile(PathUtils::NormalizePath(filePath), classesInFile);
    }
}

void ReflectionCache::UpdateCacheFromUnityFile(const UnityFileBuilder& inUnityFileBuilder)
{
    for (const std::string& path : inUnityFileBuilder.GetHeaders())
    {
        std::string normalizedPath = PathUtils::NormalizePath(path);
        
        if (myCache.find(normalizedPath) == myCache.end())
            CacheFile(normalizedPath, {});
        else
            UpdateWriteTimeForFile(normalizedPath);
    }
}

const std::unordered_map<std::string, CachedFileData>& ReflectionCache::GetCachedFiles() const
{
    return myCache;
}
