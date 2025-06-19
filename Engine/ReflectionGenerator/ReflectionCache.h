#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "ReflectedClass.h"
#include "json.hpp"
#include "UnityFileBuilder.h"

class ReflectionParser;



struct CachedFileData
{
    std::filesystem::file_time_type myLastWriteTime;
    std::list<ReflectedClass> myClasses;

    long long FileTimeToLongLong(const std::filesystem::file_time_type& inFileTime)
    {
        return inFileTime.time_since_epoch().count();
    }

    std::filesystem::file_time_type FileTimeFromLongLong(long long inLongLong)
    {
        return std::filesystem::file_time_type(std::filesystem::file_time_type::duration(inLongLong));
    }
    
    nlohmann::json Save()
    {
        nlohmann::json json;
        json["writeTime"] = FileTimeToLongLong(myLastWriteTime);

        for (const ReflectedClass& reflectedClass : myClasses)
        {
            json["classes"].push_back(reflectedClass.Save());
        }
        return json;
    }
    
    void Load(const nlohmann::json& inJson)
    {
        myLastWriteTime = FileTimeFromLongLong(inJson["writeTime"]);

        if (inJson.contains("classes"))
        {
            for (const nlohmann::json& json : inJson["classes"])
            {
                myClasses.emplace_back().Load(json);
            }
        }
    }
};

class ReflectionCache
{
public:
    ReflectionCache(const std::filesystem::path& inCachePath);

    void SaveCache();
    void LoadCache();

    // This will add a file to the cache potentially overwriting existing cached data.
    void CacheFile(const std::string& inFile, const std::list<ReflectedClass>& inClasses);

    void UpdateWriteTimeForFile(const std::string& inFile);

    bool IsCacheValidForFile(const std::string& inFile) const;
    const std::list<ReflectedClass>& GetCachedClassesForFile(const std::string& inFile) const;

    void UpdateCacheFromParser(const ReflectionParser& inParser);

    // We need to update the cache with unity files as some files doesnt contain any class declarations and therefore otherwise doesnt get added to the cache.
    void UpdateCacheFromUnityFile(const UnityFileBuilder& inUnityFileBuilder);

    const std::unordered_map<std::string, CachedFileData>& GetCachedFiles() const;
    
private:
    std::filesystem::path myCachePath = "";
    std::unordered_map<std::string, CachedFileData> myCache{};
};
