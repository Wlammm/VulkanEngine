#include "EnginePch.h"
#include "Asset.h"

#include "AssetRegistry.h"

Asset::~Asset()
{
    if (myIsValid)
        myAssetRegistry->OnAssetRemoved(this);
}

bool Asset::IsCacheValid() const
{
    std::filesystem::file_time_type sourceWriteTime = std::filesystem::last_write_time(GetSourcePath());
        
    // Check if source has been modified since the cache was last written.
    if (GetSourceLastModifiedTime() != sourceWriteTime)
        return false;
        
    return true;
}

void Asset::DoFirstTimeAssetInitialization(const std::filesystem::path& inAssetPath)
{
    myAssetPath = inAssetPath;
    
    if (IsExternalAsset())
    {
        check(std::filesystem::exists(inAssetPath));
        mySourceLastModifiedTime = std::filesystem::last_write_time(inAssetPath);
    }
}

void Asset::SetAssetPath(const SourcePath& inSourcePath)
{
    // TODO: Tell the asset registry that we've changed paths. Otherwise we can get assets loaded twice.
    myAssetPath = inSourcePath;
}

const SourcePath& Asset::GetSourcePath() const
{
    return myAssetPath;
}

std::string Asset::GetName() const
{
    return myAssetPath.stem().string();
}

const std::filesystem::file_time_type& Asset::GetSourceLastModifiedTime() const
{
    return mySourceLastModifiedTime;
}

void Asset::SetIsValid(const bool inIsValid)
{
    myIsValid = inIsValid;
}

void Asset::SetAssetRegistry(AssetRegistry* inAssetRegistry)
{
    myAssetRegistry = inAssetRegistry;
}

void Asset::ResaveAsset()
{
    myAssetRegistry->SaveAsset(shared_from_this());
}
