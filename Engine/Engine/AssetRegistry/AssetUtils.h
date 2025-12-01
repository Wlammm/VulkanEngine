#pragma once
#include "AssetDefines.h"

class AssetUtils
{
public:
    static void ScanAssetDirectory();
    
    static SourcePath GetSourcePathFromAssetName(const std::string& inAssetName);
    
    static List<SourcePath> GetAllSourcePathsWithExtensions(const List<std::string>& inExtensions);
    
private:
    inline static std::unordered_map<std::string, std::filesystem::path> myFilenameToPathLUT{};
};
