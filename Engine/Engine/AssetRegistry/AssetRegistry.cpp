#include "EnginePch.h"
#include "AssetRegistry.h"

#include "Asset.h"
#include "Engine/Engine.h"
#include "Engine/Core/ThreadPool.h"

AssetRegistry::AssetRegistry()
{
    
}

AssetRegistry::~AssetRegistry()
{
    myMutex.lock();

    for(const UniquePtr<IAssetContainer>& container : myContainers)
    {
        container->UnloadAllAssets();
    }
    
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