#include "EnginePch.h"
#include "AssetRegistry2.h"

#include "Asset2.h"

AssetRegistry2::AssetRegistry2()
{
    check(!mySingleton);
    mySingleton = this;
}

AssetRegistry2::~AssetRegistry2()
{
    mySingleton = nullptr;
}

CachePath AssetRegistry2::SourceToCachePath(const SourcePath& inSourcePath) const
{
    return "AssetCache/" + inSourcePath.string() + ".cache";
}

void AssetRegistry2::OnAssetRemoved(Asset2* inAsset)
{
    std::scoped_lock lock(myMutex);
    myLoadedAssets.erase(inAsset->GetSourcePath());
}

void AssetRegistry2::AddLoadedAsset(SharedPtr<Asset2> inAsset)
{
    inAsset->SetIsValid(true);
    inAsset->SetAssetRegistry(this);
    
    std::scoped_lock lock(myMutex);

    myLoadedAssets.insert({ inAsset->GetSourcePath(), inAsset });
}