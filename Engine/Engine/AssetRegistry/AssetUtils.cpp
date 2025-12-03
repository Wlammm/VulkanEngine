#include "EnginePch.h"
#include "AssetUtils.h"

#include "Asset.h"
#include "AssetRegistry.h"
#include "Engine/Reflection/ReflectionSystem.h"

void AssetUtils::Start()
{
    ScanAssetDirectory();
    BuildAssetLUTs();
}

void AssetUtils::BuildAssetLUTs()
{
    myFileExtensionToTypeLUT.clear();
    const Type* assetType = ReflectionSystem::GetType<Asset>();
    
    for (const Type* derivedAssetType : assetType->GetDerivedTypes())
    {
        Asset* instance = derivedAssetType->CreateInstance<Asset>();
        for (const std::string& extension : instance->GetAssetExtensions())
        {
            myFileExtensionToTypeLUT.insert({extension, derivedAssetType});
        }
        
        if (instance->IsExternalAsset())
            myExternalAssetTypes.Add(derivedAssetType);
        else
            myInternalAssetTypes.Add(derivedAssetType);
        
        del(instance);
    }
}

void AssetUtils::ScanAssetDirectory()
{
    myFilenameToPathLUT.clear();
    for(const std::filesystem::path& path : std::filesystem::recursive_directory_iterator("./"))
    {
        if(!path.has_extension())
            continue;

        check(!myFilenameToPathLUT.contains(path.filename().string()));
        myFilenameToPathLUT.insert({path.filename().string(), path});
    }

    LOG("AssetRegistry - Scanned %i files", myFilenameToPathLUT.size());
}

SourcePath AssetUtils::GetSourcePathFromAssetName(const std::string& inAssetName)
{
    if(inAssetName == "")
        return "";

    if(!myFilenameToPathLUT.contains(inAssetName))
        return "";
    
    return myFilenameToPathLUT.at(inAssetName);    
}

List<SourcePath> AssetUtils::GetAllSourcePathsWithExtensions(const List<std::string>& inExtensions)
{
    List<SourcePath> result;
    for (const std::pair<const std::string, std::filesystem::path>& entry : myFilenameToPathLUT)
    {
        for (const std::string& extension : inExtensions)
        {
            if (entry.second.extension() == extension)
                result.Add(entry.second);
        }
    }
    return result;
}

const List<const Type*>& AssetUtils::GetAllInternalAssetTypes()
{
    return myInternalAssetTypes;
}

const List<const Type*>& AssetUtils::GetAllExternalAssetTypes()
{
    return myExternalAssetTypes;
}

const Type* AssetUtils::GetAssetTypeFromFileExtension(const std::string& inFileExtension)
{
    return myFileExtensionToTypeLUT.at(inFileExtension);
}

std::string AssetUtils::GetExtensionForInternalType(const Type* inType)
{
    for (std::pair<const std::string, const Type*> entry : myFileExtensionToTypeLUT)
    {
        if (entry.second == inType)
            return entry.first;
    }
    
    check(false && "Failed to find asset type extension");
    return "";
}

void AssetUtils::RenameAsset(const std::filesystem::path& inCurrentPath, const std::filesystem::path& inNewPath)
{
    const Type* assetType = GetAssetTypeFromFileExtension(inCurrentPath.extension().string());
    
    SharedPtr<Asset> asset = AssetRegistry::Get()->GetAsset(inCurrentPath, assetType);
    asset->SetAssetPath(inNewPath);
    AssetRegistry::Get()->SaveAsset(asset);
    std::filesystem::remove(inCurrentPath);
}
