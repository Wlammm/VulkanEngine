#include "EnginePch.h"
#include "AssetRegistry.h"

#include "Asset.h"
#include "Engine.h"
#include "Core/ThreadPool.h"

AssetRegistry::AssetRegistry()
{
    
}

AssetRegistry::~AssetRegistry()
{
    myMutex.lock();

    for(auto [path, asset] : myLoadedAssets)
    {
        asset->Unload();
    }
    myLoadedAssets.clear();
    
    myMutex.unlock();
}

void AssetRegistry::ScanAssetsFolder()
{
    for(const std::filesystem::path& path : std::filesystem::recursive_directory_iterator("./"))
    {
        if(!path.has_extension())
            continue;

        check(!myFilenameToPathLUT.contains(path.filename().string()));
        myFilenameToPathLUT.insert({path.filename().string(), path});
    }

    LOG("AssetRegistry - Scanned %i files", myFilenameToPathLUT.size());
}

std::filesystem::path AssetRegistry::GetPathFromAssetName(const std::string& inAssetName)
{
    if(inAssetName == "")
        return "";

    if(!myFilenameToPathLUT.contains(inAssetName))
        return "";
    
    return myFilenameToPathLUT.at(inAssetName);    
}

bool AssetRegistry::HasAsset(const std::filesystem::path& inAssetPath) const
{
    return myLoadedAssets.contains(inAssetPath);
}

void AssetRegistry::RegisterTemporaryAsset(const std::filesystem::path& inAssetPath, Asset* inAsset)
{
    inAsset->myAssetRegistry = this;
    myMutex.lock();
    if(myLoadedAssets.contains(inAssetPath))
    {
        myMutex.unlock();
        return;
    }

    myLoadedAssets.insert({inAssetPath, inAsset});
    myMutex.unlock();
}
