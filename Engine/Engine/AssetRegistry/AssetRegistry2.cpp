#include "EnginePch.h"
#include "AssetRegistry2.h"

Asset2::~Asset2()
{
    if (myIsValid)
        myAssetRegistry->OnAssetRemoved(this);
}

void Asset2::DoFirstTimeAssetInitialization(const std::filesystem::path& inAssetPath)
{
    check(std::filesystem::exists(inAssetPath) && "Invalid asset path sent into");
    myAssetPath = inAssetPath;
    mySourceLastModifiedTime = std::filesystem::last_write_time(inAssetPath);
}

const SourcePath& Asset2::GetSourcePath()
{
    return myAssetPath;
}

const std::filesystem::file_time_type& Asset2::GetSourceLastModifiedTime() const
{
    return mySourceLastModifiedTime;
}

void Asset2::SetIsValid(const bool inIsValid)
{
    myIsValid = inIsValid;
}

void Asset2::SetAssetRegistry(AssetRegistry2* inAssetRegistry)
{
    myAssetRegistry = inAssetRegistry;
}

CachePath AssetRegistry2::SourceToCachePath(const SourcePath& inSourcePath) const
{
    return "AssetCache/" + inSourcePath.string() + ".cache";
}

void AssetRegistry2::OnAssetRemoved(Asset2* inAsset)
{
    std::scoped_lock lock(myMutex);
    LOG("Locked in AssetRegistry2::OnAssetRemoved");
    myLoadedAssets.erase(inAsset->GetSourcePath());
}

void AssetRegistry2::AddLoadedAsset(SharedPtr<Asset2> inAsset)
{
    inAsset->SetIsValid(true);
    inAsset->SetAssetRegistry(this);
    
    std::scoped_lock lock(myMutex);
    LOG("Locked in AssetRegistry2::AddLoadedAsset");

    myLoadedAssets.insert({ inAsset->GetSourcePath(), inAsset });
}