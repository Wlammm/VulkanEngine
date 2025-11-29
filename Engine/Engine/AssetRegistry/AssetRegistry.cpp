#include "EnginePch.h"
#include "AssetRegistry.h"

#include "Asset.h"

AssetRegistry::AssetRegistry()
{
    check(!mySingleton);
    mySingleton = this;
}

AssetRegistry::~AssetRegistry()
{
    mySingleton = nullptr;
}

CachePath AssetRegistry::SourceToCachePath(const SourcePath& inSourcePath) const
{
    return "AssetCache/" + inSourcePath.string() + ".cache";
}

void AssetRegistry::OnAssetRemoved(Asset* inAsset)
{
    std::scoped_lock lock(myMutex);
    myLoadedAssets.erase(inAsset->GetSourcePath());
}

void AssetRegistry::AddLoadedAsset(SharedPtr<Asset> inAsset)
{
    inAsset->SetIsValid(true);
    inAsset->SetAssetRegistry(this);
    
    std::scoped_lock lock(myMutex);

    myLoadedAssets.insert({ inAsset->GetSourcePath(), inAsset });
}