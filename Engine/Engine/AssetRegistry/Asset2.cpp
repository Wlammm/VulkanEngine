#include "EnginePch.h"
#include "Asset2.h"

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

void Asset2::ResaveAsset()
{
    myAssetRegistry->SaveAssetToCache(shared_from_this());
}
