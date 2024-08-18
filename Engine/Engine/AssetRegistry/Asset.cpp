#include "EnginePch.h"
#include "Asset.h"

bool Asset::IsValid() const
{
    return myIsValid;    
}

AssetRegistry* Asset::GetAssetRegistry() const
{
    return myAssetRegistry;
}