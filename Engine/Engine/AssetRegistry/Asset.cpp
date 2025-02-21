#include "EnginePch.h"
#include "Asset.h"

bool Asset::IsValid() const
{
    return myIsValid;    
}

const std::filesystem::path Asset::GetPath() const
{
    return myPath;
}

AssetRegistry* Asset::GetAssetRegistry() const
{
    return myAssetRegistry;
}
