#pragma once
#include "AssetDefines.h"

/*
 * A static utility library for assets.
 */
class AssetUtils
{
public:
    static void Start();
    
    static SourcePath GetSourcePathFromAssetName(const std::string& inAssetName);
    
    static List<SourcePath> GetAllSourcePathsWithExtensions(const List<std::string>& inExtensions);
    
    static const List<const Type*>& GetAllInternalAssetTypes();
    static const List<const Type*>& GetAllExternalAssetTypes();

    static const Type* GetAssetTypeFromFileExtension(const std::string& inFileExtension);
    
    static std::string GetExtensionForInternalType(const Type* inType);
    
    // Use to change path or name of an asset. 
    static void RenameAsset(const std::filesystem::path& inCurrentPath, const std::filesystem::path& inNewPath);
    
private:
    static void BuildAssetLUTs();
    static void ScanAssetDirectory();
    
private:
    inline static std::unordered_map<std::string, std::filesystem::path> myFilenameToPathLUT{};
    inline static std::unordered_map<std::string, const Type*> myFileExtensionToTypeLUT{};
    inline static List<const Type*> myInternalAssetTypes{};
    inline static List<const Type*> myExternalAssetTypes{};
};
